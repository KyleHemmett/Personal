from cProfile import label
from sklearn.metrics import classification_report
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
import numpy as np
import pandas as pd
from sklearn.svm import SVC
from sklearn.tree import DecisionTreeClassifier  # Import Decision Tree Classifier
from sklearn.neighbors import KNeighborsClassifier, KNeighborsRegressor

# evaluate adaboost algorithm for classification
from numpy import mean
from numpy import std
from sklearn.datasets import make_classification
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.ensemble import AdaBoostClassifier
from sklearn import metrics as sm
from sklearn import tree
import matplotlib.pyplot as plt


# load the dataset
df = pd.read_csv("Extracted Data.csv")
X, y = df.iloc[:, :-1], df.iloc[:, -1]


X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.1, random_state=4)

y_values = y_test.to_numpy()

sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)

k_values = [*range(1, 50, 1)]
r2_score1 = []
r2_score2 = []

# for k_val in k_values:
#     classifier = KNeighborsRegressor(n_neighbors=k_val)
#     classifier.fit(X_train, y_train.values.ravel())
#     predictions = classifier.predict(X_test)
#     r2_score1.append(round(sm.r2_score(y_test, predictions), 2))
#     classifier = KNeighborsRegressor(n_neighbors=k_val, weights="distance")
#     classifier.fit(X_train, y_train.values.ravel())
#     predictions = classifier.predict(X_test)
#     print(predictions)
#     print(y_test)
#     r2_score2.append(round(sm.r2_score(y_test, predictions), 2))

classifier = KNeighborsRegressor(n_neighbors=13, weights="distance")
classifier.fit(X_train, y_train.values.ravel())
predictions = classifier.predict(X_test)

predictions = [round(num, 1) for num in predictions]


plt.plot(predictions)
plt.plot(np.array(y_test))
plt.show()


# plt.title("title")
# plt.xlabel("K Neighbours")
# plt.ylabel("R2 Score")
# plt.plot(k_values, r2_score1, color="blue", label="r2 score with uniform weighting")
# plt.plot(k_values, r2_score2, color="red", label="r2_score with distance weighting")
# plt.legend()
# plt.show()
