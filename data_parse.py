import os
import re
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from sklearn.linear_model import LinearRegression
directories = []

for root, dirs, files in os.walk("data"):
    if not dirs:
        directories.append(root)


experiments = []

for directory in directories:
    # print(directory)
    experiment_settings = list(map(int, re.findall(r'\d+', directory)))
    # print(experiment_settings)
    experiment = {
        "num_robots": experiment_settings[0],
        "aperture": experiment_settings[1],
        "distance": 45,
        "trials": []
    }

    # trials
    files = os.listdir(directory)

    for file in files:
        trial_settings = list(map(int, re.findall(r'\d+', file)))
        trial = {
            "number": trial_settings[0],
            "expected_faulty_robot": 0,
            "actual_faulty_robot": 0,
            "induced_fault_step": 0,
            "detected_fault_step": 0,
            "left_wheel": 0,
            "right_wheel": 0,


        }
        with open(os.path.join(directory, file), 'r') as f:
            raw_data = f.readlines()

        data = []
        for line in raw_data:
            data.append([int(i) for i in line.strip('\n').split('\t')])
        line = data[-1]
        trial["expected_faulty_robot"] = line[1]
        trial["actual_faulty_robot"] = line[7]
        trial["induced_fault_step"] = line[4]
        trial["detected_fault_step"] = line[5]
        trial["left_wheel"] = line[8]
        trial["right_wheel"] = line[9]
        experiment["trials"].append(trial)
    experiments.append(experiment)


print(len(experiments))
true_positive_speed = []
false_positive_speed = []
true_positive_left_wheel = []
true_positive_right_wheel = []
false_positive_left_wheel = []
false_positive_right_wheel = []
trials_speed_detection = []

labels = []
for experiment in experiments:
    labels.append("#: {num_robots} a: {aperture}".format(num_robots=experiment["num_robots"], aperture=experiment["aperture"]))
    trial_speed_detection = []
    for trial in experiment["trials"]:
        trial_speed_detection.append(trial["detected_fault_step"] - trial["induced_fault_step"])
        if not trial["expected_faulty_robot"] == trial["actual_faulty_robot"]:
            false_positive_left_wheel.append(trial["left_wheel"])
            false_positive_right_wheel.append(trial["right_wheel"])
            false_positive_speed.append(trial["detected_fault_step"] - trial["induced_fault_step"])
        else:
            true_positive_left_wheel.append([trial["left_wheel"]])
            true_positive_right_wheel.append([trial["right_wheel"]])
            true_positive_speed.append(trial["detected_fault_step"] - trial["induced_fault_step"])
    trials_speed_detection.append(trial_speed_detection)


model = LinearRegression()
model.fit(np.expand_dims(np.array(false_positive_right_wheel), 1),
          np.expand_dims(np.array(false_positive_left_wheel), 1))

x_new = np.linspace(-6, 6)
y_new = model.predict(x_new[:, np.newaxis])

figure = plt.figure()
ax = plt.axes()
ax.boxplot(trials_speed_detection, vert=False, labels=labels)
ax.set_title("Detection Time per Experiment")
ax.set_xlabel("Detection Time (ticks)")
ax.set_ylabel("Experiments")
figure.show()

figure = plt.figure()
ax = figure.add_subplot()
ax.scatter(true_positive_right_wheel, true_positive_left_wheel, c=["b"], label="TP")
ax.scatter(false_positive_right_wheel, false_positive_left_wheel, c=["r"], label="Fp")
ax.plot(x_new, y_new, 'r')
ax.set_title("Faulty Robot Wheel Speed")
ax.set_ylabel("Left Wheel (cm/s)")
ax.set_xlabel("Right Wheel (cm/s)")
ax.legend()
figure.show()

figure = plt.figure()
ax = plt.axes()
ax.boxplot([true_positive_speed, false_positive_speed], vert=False, labels=["TP", "FP"])
ax.set_title("Detection Time of All Trials")
ax.set_xlabel("Detection Time (ticks)")
ax.set_ylabel("Detection Type")

figure.show()


figure = plt.figure()
ax = Axes3D(figure)
ax.scatter(true_positive_left_wheel, true_positive_right_wheel, zs=np.array(true_positive_speed))
ax.set_title("Detection Time vs. Faulty Robot Wheel Speed")
ax.set_xlabel("Left Wheel (cm/s)")
ax.set_ylabel("Right Wheel (cm/s)")
ax.set_zlabel("Detection Time (ticks)")
plt.show()