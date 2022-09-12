
from sklearn.metrics import classification_report
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.svm import SVR
from sklearn.decomposition import PCA
import numpy as np
import pandas as pd
from sklearn import metrics as sm
import matplotlib.pyplot as plt
from sklearn.ensemble import AdaBoostClassifier


df = pd.read_csv("Extracted Data.csv")
X, y = df.iloc[:, :-1], df.iloc[:, -1]

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.25, random_state=4
)


sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)

C_vlaues = [0.01, 1, 10, 100]
print("output")
print("RBF Kernel")

gamma_values = [0.1, 1, 10]

rbf = SVR(kernel="rbf", gamma=0.1, C=100).fit(X_train, y_train.values.ravel())
predictions = rbf.predict(X_test)
print(
    "Accuracy(gamma=%f,C=%f): %f"
    % (0.1, 100, round(sm.r2_score(y_test, predictions), 2))
)

length = len(predictions)

predictions = [round(num, 1) for num in predictions]


plt.plot(predictions)
plt.plot(np.array(y_test))
plt.show()


print(predictions, 1)
print(np.array(y_test))
