import pydicom
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
import matplotlib.image as img
import pandas as pd
import os


class Dicom:
    def __init__(self, file) -> None:
        self.df = pydicom.read_file(file)
        self.manufacturer_model_name = self.df.ManufacturerModelName
        self.RT_plan_name = self.df.RTPlanName
        self.patient_ID = self.df.PatientID
        self.patient_name = self.df.PatientName
        self.patient_birth_date = self.df.PatientBirthDate
        self.patient_sex = self.df.PatientSex
        self.RT_plan_date = self.df.RTPlanDate
        self.set_delivery()
        self.set_sinogram()
        self.set_lot_values()
        self.create_final_array()

    # Sets all Delivery Variables
    def set_delivery(self):
        # Read the number of projections(Nproj)
        self.Nproj = self.df.BeamSequence[0].NumberOfControlPoints
        # Define the rotating direction
        self.gantry_rotation_direction = (
            self.df.BeamSequence[0].ControlPointSequence[0].GantryRotationDirection
        )
        # Allocate the vector with the gantry angles
        self.gantry_angle = np.zeros(self.Nproj)
        # Import number of leaves
        self.num_leaves = 64
        # Import information at each CP
        self.sinogram = []
        # Number of Projections per rotation (Nproj,rot)
        self.Nprojrot = 51
        # Number of Rotations(Nrot)
        self.Nrot = (self.Nproj - 1) / self.Nprojrot
        # Gantry Period(GP) s
        self.GP = float(
            (str(self.df.BeamSequence[0][0x300D, 0x1040].value).strip())[2:-2]
        )
        # projection Time(PT) s
        self.PT = self.GP / self.Nprojrot
        # treatment Time(TT) in (s)
        self.TT = self.GP * self.Nrot
        # Couch Speed(CS) mm/s
        self.CS = float(
            (str(self.df.BeamSequence[0][0x300D, 0x1080].value).strip())[2:-2]
        )
        # Couch Translation(CT) mm
        self.CT = self.CS * self.TT
        # pitch
        self.pitch = float(
            (str(self.df.BeamSequence[0][0x300D, 0x1060].value).strip())[2:-2]
        )
        # field width(FW) mm
        self.FW = self.CT / self.Nrot / self.pitch
        # target length (TL) mm
        self.TL = self.CT - self.FW

    # creates sinogram
    def set_sinogram(self):
        for cp in range(self.Nproj - 1):
            self.gantry_angle[cp] = (
                self.df.BeamSequence[0].ControlPointSequence[cp].GantryAngle
            )
            tmp = str(
                self.df.BeamSequence[0].ControlPointSequence[cp][0x300D, 0x10A7].value
            ).strip()
            tmp = tmp[2:-1]
            tmp = tmp.split("\\")
            leaves_contol_point = []
            for i in tmp:
                if i == "" or i == "n":
                    continue
                else:
                    leaves_contol_point.append(float(i))

            if len(leaves_contol_point) == 0:
                self.sinogram.append(np.zeros(self.num_leaves))
            else:
                self.sinogram.append((leaves_contol_point))

    def set_lot_values(self):
        max_leaf_opening_time = 0
        total_leaf_opening_time = 0
        self.non_zero_values = []
        for i in range(len(self.sinogram)):
            for j in range(len(self.sinogram[0])):
                total_leaf_opening_time = total_leaf_opening_time + self.sinogram[i][j]
                if self.sinogram[i][j] > max_leaf_opening_time:
                    max_leaf_opening_time = self.sinogram[i][j]
                if self.sinogram[i][j] != 0:
                    self.non_zero_values.append(self.sinogram[i][j])

        average_leaf_opening_time = total_leaf_opening_time / i

        # Modulation Factor(MF)
        self.MF = average_leaf_opening_time / max_leaf_opening_time

        # Mean leaf opening time (mLOT) * projection time
        self.mLOT = np.mean(self.non_zero_values) * (self.PT * 1000)

        # Mean leaf opening (mFLOT)
        self.mFLOT = np.mean(self.non_zero_values)

        # standrd dev leaf opening time (sdLOT) * projection time
        self.sdLOT = np.std(self.non_zero_values) * (self.PT * 1000)

        # standrd dev leaf opening time (sdFLOT)
        self.sdFLOT = np.std(self.non_zero_values)

        # median leaf opening time (mdLOT) * projection time
        self.mdLOT = np.median(self.non_zero_values) * (self.PT * 1000)

        # median leaf opening time (mdFLOT)
        self.mdFLOT = np.median(self.non_zero_values)

        # mode leaf opening time (moLOT) * projection time
        self.moLOT = (
            stats.mode((np.multiply(np.array(self.non_zero_values), self.PT * 1000)))
        )[0][0]

        # mode leaf opening time (moFLOT)
        self.moFLOT = (stats.mode(np.array(self.non_zero_values)))[0][0]

        # maximum leaf opening time (maxLOT) * projection time
        self.maxLOT = np.amax(self.non_zero_values) * (self.PT * 1000)

        # maximum leaf opening time (maxFLOT)
        self.maxFLOT = np.amax(self.non_zero_values)

        # minimum leaf opening time (minLOT) * projection time
        self.minLOT = np.amin(self.non_zero_values) * (self.PT * 1000)

        # minimum leaf opening time (minFLOT)
        self.minFLOT = np.amin(self.non_zero_values)

        # kurtosis leaf opening time (kLOT) * projection time
        self.kLOT = stats.kurtosis(self.non_zero_values)

        # skewness leaf opening time (sLOT) * projection time
        self.sLOT = stats.skew(self.non_zero_values)

        self.set_sinogram_values(self.non_zero_values)

    def set_sinogram_values(self, non_zero_values):

        self.CLNS20 = self.clns(non_zero_values, 20)

        self.CLNS30 = self.clns(non_zero_values, 30)

        self.CLNS50 = self.clns(non_zero_values, 50)

        self.CLNS100 = self.clns(non_zero_values, 100)

        self.CLNSpt20 = self.clnspt(non_zero_values, 20)

        self.CFNS5 = self.cfns(non_zero_values, 5)

        self.CFNS10 = self.cfns(non_zero_values, 10)

        self.CFNS25 = self.cfns(non_zero_values, 25)

        self.CFNS50 = self.cfns(non_zero_values, 50)

        self.CFNS75 = self.cfns(non_zero_values, 75)

        self.CFNS90 = self.cfns(non_zero_values, 90)

        self.CFNS95 = self.cfns(non_zero_values, 95)

        # lengthCC,
        (
            self.nCC,
            self.TA,
            self.fDisk,
            self.CLS,
            self.CLSinareadisk,
            self.CLSinarea,
            self.CLSindisk,
            self.CLSin,
            self.centroid,
        ) = self.get_geomentry()

        self.PSTV = self.get_PSTV()

        self.EPSTV10 = self.get_EPSTV(1, 0)

        self.EPSTV01 = self.get_EPSTV(0, 1)

        self.EPSTV11 = self.get_EPSTV(1, 1)

        self.LOTV = self.get_LOTV()

        self.ELOTV1 = self.get_ELOTV(1)

        self.ELOTV3 = self.get_ELOTV(3)

        self.ELOTV5 = self.get_ELOTV(5)

        self.MI = self.get_MI()

        self.mSI, self.mdSI, self.sdSI = self.get_SI()

        self.MSA = self.get_MSA()

    def get_geomentry(self):
        centroids = []
        nCCs = []
        treatment_area_array = []
        # lengthCCs = []
        closed_leaves_in = []
        CLSinarea_array = []
        clacps = []
        clin_array = []
        for i in self.sinogram:
            zeros = len(i) - np.count_nonzero(i)
            clacps.append(zeros / len(i))
            index_non_zero = np.where(np.array(i) > 0)[0]
            if len(index_non_zero) != 0:
                index_zero = np.where(
                    np.array(i[index_non_zero[0] : index_non_zero[-1]]) == 0
                )[0]
                index_zero = [index_non_zero[0]] + index_zero + [index_non_zero[-1]]
                connected_comps = np.array(np.diff(index_zero))
                connected_comps[connected_comps > 0]
                # lengthCCs.append(connected_comps[0])
                nCCs.append(len(connected_comps))
                treatment_area_value = index_non_zero[-1] - index_non_zero[0] + 1
                treatment_area_array.append(treatment_area_value)
                closed_leaf_in = np.count_nonzero(
                    i[index_non_zero[0] : index_non_zero[-1]]
                )
                closed_leaves_in.append(closed_leaf_in)
                CLSinarea_array.append(closed_leaf_in / treatment_area_value)
                clin_array.append(closed_leaf_in / self.num_leaves)
                centroids.append(
                    np.mean(np.abs(index_non_zero - ((self.num_leaves / 2) + 0.5)))
                )

        nCC = np.mean(nCCs)
        # lengthCC = np.mean(lengthCCs)
        TA = np.mean(treatment_area_array)
        fDisk = np.count_nonzero(closed_leaves_in) / self.Nproj
        CLS = np.mean(clacps)
        CLSinareadisk = np.mean(np.nonzero(CLSinarea_array))
        CLSinarea = np.mean(CLSinarea_array)
        CLSindisk = np.mean(np.nonzero(clin_array))
        CLSin = np.mean(clin_array)
        centroid = np.mean(centroids)

        # lengthCC,
        return (
            nCC,
            TA,
            fDisk,
            CLS,
            CLSinareadisk,
            CLSinarea,
            CLSindisk,
            CLSin,
            centroid,
        )

    def clns(self, non_zero_values, checkpoint):
        num_passing_values = 0
        for i in non_zero_values:
            if (i * (self.PT * 1000)) < checkpoint:
                num_passing_values += 1

        return num_passing_values / len(non_zero_values)

    def clnspt(self, non_zero_values, checkpoint):
        num_passing_values = 0
        for i in non_zero_values:
            if (i * (self.PT * 1000)) < ((self.PT * 1000) - checkpoint):
                num_passing_values += 1

        return num_passing_values / len(non_zero_values)

    def cfns(self, non_zero_values, checkpoint):
        num_passing_values = 0
        for i in non_zero_values:
            if i * (self.PT) < checkpoint / 100:
                num_passing_values += 1

        return num_passing_values / len(non_zero_values)

    def get_PSTV(self):
        PSTV = np.array(
            np.sum(
                np.abs(
                    np.array(
                        (self.sinogram[0:-3][0:-2])
                        - np.array(self.sinogram[0:-3][1:-1])
                    )
                ),
                1,
            )
            + np.sum(
                np.abs(
                    np.array(
                        (self.sinogram[0:-3][0:-2])
                        - np.array(self.sinogram[1:-2][0:-2])
                    )
                ),
                1,
            )
        )

        return np.mean(PSTV)

    def get_EPSTV(self, p, l):
        EPSTV = np.array(
            np.sum(
                np.abs(
                    np.array(
                        (self.sinogram[0 : -l - 2][0 : -1 - p])
                        - np.array(self.sinogram[0 : -l - 2][0 + p : -1])
                    )
                ),
                1,
            )
            + np.sum(
                np.abs(
                    np.array(
                        (self.sinogram[0 : -l - 2][0 : -1 - p])
                        - np.array(self.sinogram[0 + l : -2][0 : -1 - p])
                    )
                ),
                1,
            )
        )
        return np.mean(EPSTV)

    def get_LOTV(self):
        LOTV_array = np.zeros(64)
        tmax = np.amax(self.sinogram, axis=0)
        for i in range(self.num_leaves):
            for j in range(self.Nproj - 2):
                LOTV_array[i] = (
                    LOTV_array[i]
                    + tmax[i]
                    - np.abs(self.sinogram[j][i])
                    - np.abs(self.sinogram[j + 1][i])
                )
            LOTV_array[i] = LOTV_array[i] / (self.Nproj - 2)
            # LOTV_array[i] = LOTV_array[i] / tmax[i]

        return np.mean(LOTV_array)

    def get_ELOTV(self, input):
        LOTV_array = np.zeros(64)
        tmax = np.amax(self.sinogram, axis=0)
        for i in range(self.num_leaves):
            for j in range(self.Nproj - input - 1):
                LOTV_array[i] = (
                    LOTV_array[i]
                    + tmax[i]
                    - np.abs(self.sinogram[j][i])
                    - np.abs(self.sinogram[j + input][i])
                )
            LOTV_array[i] = LOTV_array[i] / (self.Nproj - input - 1)
            # LOTV_array[i] = LOTV_array[i] / tmax[i]

        return np.mean(LOTV_array)

    def get_MI(self):
        Zx = [0, 0, 0]
        Zy = [0, 0, 0]
        Zxy = [0, 0, 0]
        Zyx = [0, 0, 0]
        ff = [0.01, 0.01, 2]

        sigma = np.std(self.sinogram) * self.PT

        Deltatx = (
            np.abs(
                np.array(self.sinogram[0:-2][1:-1])
                - np.array(self.sinogram[0:-2][0:-2])
            )
            * self.PT
        )
        Deltaty = (
            np.abs(np.array(self.sinogram[1:-2][:]) - np.array(self.sinogram[0:-3][:]))
            * self.PT
        )
        Deltatxy = (
            np.abs(
                np.array(self.sinogram[1:-2][1:-1])
                - np.array(self.sinogram[0:-3][0:-2])
            )
            * self.PT
        )
        Deltatyx = (
            np.abs(
                np.array(self.sinogram[0:-3][1:-1])
                - np.array(self.sinogram[1:-2][0:-2])
            )
            * self.PT
        )

        for i in range(len(ff)):
            Zx[i] = np.count_nonzero(Deltatx > ff[i] * sigma) / self.Nproj
            Zy[i] = np.count_nonzero(Deltaty > ff[i] * sigma) / self.Nproj
            Zxy[i] = np.count_nonzero(Deltatxy > ff[i] * sigma) / self.Nproj
            Zyx[i] = np.count_nonzero(Deltatyx > ff[i] * sigma) / self.Nproj

        Zf = np.array(Zx + Zy + Zxy + Zyx) / 4

        mi = np.sum(Zf)
        return mi

    def get_SI(self):
        mSI = np.mean(np.nonzero(np.sum(self.sinogram, axis=0) * self.PT / self.TT))
        mdSI = np.median(np.nonzero(np.sum(self.sinogram, axis=0) * self.PT / self.TT))
        sdSI = np.std(np.nonzero(np.sum(self.sinogram, axis=0) * self.PT / self.TT))

        return mSI, mdSI, sdSI

    def get_MSA(self):
        Pj = np.arange(self.num_leaves) - self.num_leaves / 2 + 1.5
        lps = np.sum(self.sinogram, 0) * self.PT / self.TT

        MSA = (np.sum(np.multiply(lps, Pj))) / (np.sum(lps))

        return MSA

    # creates masked singogram (if value > 0 value = 1)
    def create_mask_sinogram(self):
        masked_sinogram = self.sinogram
        for i in range(len(masked_sinogram)):
            for j in range(len(masked_sinogram[0])):
                if masked_sinogram[i][j] > 0:
                    masked_sinogram[i][j] = 1

        return masked_sinogram

    # creates leaf array
    def create_leaf_array(self):
        leaf_array = []

        for i in range(len(self.sinogram[0])):
            value = 0
            for j in range(len(self.sinogram)):
                value = value + self.sinogram[j][i]
            average_leaf = value / j
            leaf_array.append((average_leaf))
        return leaf_array

    # Plot leaf array
    def plot_leaf_array(leaf_array):
        total_leaf_list = list(range(1, 65))
        plt.bar(total_leaf_list, leaf_array)
        plt.show()

    # plots sinogram
    def plot_sinogram(sinogram):
        plt.imshow(sinogram)
        plt.show()

    def create_final_array(self):
        self.final_array = [
            self.pitch,
            self.FW,
            self.PT,
            self.GP,
            self.TT,
            self.TL,
            self.CS,
            self.CT,
            self.Nproj,
            self.Nrot,
            self.Nprojrot,
            self.MF,
            # TTDF
            self.mLOT,
            self.sdLOT,
            self.mdLOT,
            self.moLOT,
            self.kLOT,
            self.sLOT,
            self.minLOT,
            self.maxLOT,
            self.CLNS100,
            self.CLNS50,
            self.CLNS30,
            self.CLNS20,
            self.CLNSpt20,
            self.mFLOT,
            self.sdFLOT,
            self.mdFLOT,
            self.moFLOT,
            self.minFLOT,
            self.maxFLOT,
            self.CFNS5,
            self.CFNS10,
            self.CFNS25,
            self.CFNS50,
            self.CFNS75,
            self.CFNS90,
            self.CFNS95,
            # l0NS
            # L1NS
            # L2NS
            self.nCC,
            # lengthCC
            self.TA,
            self.fDisk,
            self.CLS,
            self.CLSin,
            self.CLSinarea,
            self.CLSinareadisk,
            self.centroid,
            # nOC,
            self.PSTV,
            self.EPSTV11,
            self.EPSTV01,
            self.EPSTV10,
            self.LOTV,
            self.ELOTV1,
            self.ELOTV3,
            self.ELOTV5,
            self.MI,
            self.mSI,
            self.mdSI,
            self.sdSI,
            self.MSA,
        ]


def read_results():
    path = "C:\\Users\\User\\OneDrive\\Documents\\Engineering\\2022\\ENGG4811 - Thesis\\Dicom Files\\TomoTherapy plans\\results_editted 2021Q4.txt"

    file = open(path, "r")

    lines = file.readlines()

    patient_array = []
    finalarray = []

    current_patient = 0

    for line in lines:
        if line.startswith("patient"):
            line = line.replace(".pdf", "")
            line = line.replace("patient", "")
            current_patient = int(line)

        elif line.startswith("DTA"):
            tmp = []
            patient_array.append(current_patient)
            for t in line.split():
                try:
                    tmp.append(float(t))
                except ValueError:
                    pass
            finalarray.append(tmp)

    df = pd.DataFrame(finalarray, columns=["DTA", "DD", "Pass%"])

    df = df.reset_index(drop=True)

    return patient_array, df


pateint_array, results = read_results()


csv_array = []

rootdir = "C:\\Users\\User\\OneDrive\\Documents\\Engineering\\2022\\ENGG4811 - Thesis\\Dicom Files\\TomoTherapy plans\\RP FILES 2021Q4"

cols = [
    "pitch",
    "FW",
    "PT",
    "GP",
    "TT",
    "TL",
    "CS",
    "CT",
    "Nproj",
    "Nrot",
    "Nprojrot",
    "MF",
    # TTDF
    "mLOT",
    "sdLOT",
    "mdLOT",
    "moLOT",
    "kLOT",
    "sLOT",
    "minLOT",
    "maxLOT",
    "CLNS100",
    "CLNS50",
    "CLNS30",
    "CLNS20",
    "CLNSpt20",
    "mFLOT",
    "sdFLOT",
    "mdFLOT",
    "moFLOT",
    "minFLOT",
    "maxFLOT",
    "CFNS5",
    "CFNS10",
    "CFNS25",
    "CFNS50",
    "CFNS75",
    "CFNS90",
    "CFNS95",
    # l0NS
    # L1NS
    # L2NS
    "nCC",
    # lengthCC
    "TA",
    "fDisk",
    "CLS",
    "CLSin",
    "CLSinarea",
    "CLSinareadisk",
    "centroid",
    # nOC",
    "PSTV",
    "EPSTV11",
    "EPSTV01",
    "EPSTV10",
    "LOTV",
    "ELOTV1",
    "ELOTV3",
    "ELOTV5",
    "MI",
    "mSI",
    "mdSI",
    "sdSI",
    "MSA",
]

rootpath = "C:\\Users\\User\\OneDrive\\Documents\\Engineering\\2022\\ENGG4811 - Thesis\\Dicom Files\\TomoTherapy plans\\RP FILES 2021Q4\\plan"
count = 0
for i in pateint_array:
    try:
        print(str(i))
        subrootpath = rootpath + str(i)
        file = os.listdir(subrootpath)
        final_path = subrootpath + "\\" + file[0]
        tmp = Dicom(final_path)
        csv_array.append(tmp.final_array)
        print(count)
    except:
        print("jere")
        results.drop(count)
        continue
    finally:
        count += 1


dicomdata = pd.DataFrame(csv_array, columns=cols)

final_file = pd.concat([dicomdata, results], axis=1)

final_file.to_csv("finalfile2.csv", index=False)
