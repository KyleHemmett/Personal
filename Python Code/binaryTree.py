from sklearn.metrics import classification_report
from sklearn.preprocessing import StandardScaler
from sklearn.model_selection import train_test_split
from sklearn.svm import SVC
import numpy as np
import pandas as pd
from keras.models import Sequential
from keras.layers import Dense
from sklearn.svm import SVC
from sklearn.tree import (
    DecisionTreeClassifier,
    DecisionTreeRegressor,
)  # Import Decision Tree Classifier

# evaluate adaboost algorithm for classification
from numpy import mean
from sklearn.linear_model import LinearRegression
from numpy import std
from sklearn.datasets import make_classification
from sklearn.model_selection import cross_val_score
from sklearn.model_selection import RepeatedStratifiedKFold
from sklearn.ensemble import RandomForestRegressor
from sklearn.ensemble import AdaBoostClassifier, AdaBoostRegressor
from sklearn import metrics as sm
from sklearn import tree
import matplotlib.pyplot as plt


df = pd.read_csv("Extracted Data.csv")
X, y = df.iloc[:, :-1], df.iloc[:, -1]

X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.25, random_state=4
)

y_values = y_test.to_numpy()

sc = StandardScaler()
X_train = sc.fit_transform(X_train)
X_test = sc.transform(X_test)

depth_values = [*range(2, 50, 1)]
r2_score = []
r2_score2 = []
r2_score3 = []

for depth_value in depth_values:
    decTree = DecisionTreeRegressor(max_depth=1)

    abc = AdaBoostRegressor(
        n_estimators=depth_value,
        base_estimator=decTree,
        learning_rate=1,
        loss="linear",
    )
    abc.fit(X_train, y_train.values.ravel())
    predictions = abc.predict(X_test)
    r2_score.append(round(sm.r2_score(y_test, predictions), 2))

    abc = AdaBoostRegressor(
        n_estimators=depth_value,
        base_estimator=decTree,
        learning_rate=1,
        loss="square",
    )
    abc.fit(X_train, y_train.values.ravel())
    predictions = abc.predict(X_test)
    r2_score2.append(round(sm.r2_score(y_test, predictions), 2))

    abc = AdaBoostRegressor(
        n_estimators=depth_value,
        base_estimator=decTree,
        learning_rate=1,
        loss="exponential",
    )
    abc.fit(X_train, y_train.values.ravel())
    predictions = abc.predict(X_test)
    r2_score3.append(round(sm.r2_score(y_test, predictions), 2))

plt.title("out")
plt.xlabel("Num of Estimators")
plt.ylabel("R2 Score")
plt.plot(depth_values, r2_score, color="red", label="with linear loss")
plt.plot(depth_values, r2_score2, color="blue", label="with square loss")
plt.plot(depth_values, r2_score3, color="green", label="with exponential loss")
plt.legend()
plt.show()
