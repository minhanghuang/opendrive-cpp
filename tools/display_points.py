import math
import numpy as np
import matplotlib.pyplot as plt


def untie_list(data: list):
    xs = []
    ys = []
    for item in data:
        xs.append(item[0])
        ys.append(item[1])
    return xs, ys


if __name__ == "__main__":
    print("display.")
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    line_x_list = [1, 2, 3, 4, 5]
    line_y_list = [1, 2, 3, 4, 5]
    # data = [
    #     [1, 1],
    #     [2, 2],
    #     [3, 3],
    #     [4, 4],
    #     [5, 5],
    # ]
    # line_x_list, line_y_list = untie_list(data)
    ax.plot(line_x_list, line_y_list)
    ax.legend(['line'])
    plt.show()
