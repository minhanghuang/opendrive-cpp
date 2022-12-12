import math
import numpy as np
import matplotlib.pyplot as plt
import xml.etree.ElementTree as ET


def ParseMapFile(file_path):
    tree = ET.parse(file_path)
    root = tree.getroot()
    left_xs = []
    left_ys = []
    right_xs = []
    right_ys = []
    reference_xs = []
    reference_ys = []
    for child in root:
        if child.tag == "road":
            for section in child:
                for lane in section:
                    if lane.tag == "lane":
                        for line in lane:
                            if line.tag == "left_boundary":
                                left_x = []
                                left_y = []
                                for point in line:
                                    left_x.append(float(point.attrib.get("x")))
                                    left_y.append(float(point.attrib.get("y")))
                                left_xs.append(left_x)
                                left_ys.append(left_y)
                            elif line.tag == "right_boundary":
                                right_x = []
                                right_y = []
                                for point in line:
                                    right_x.append(
                                        float(point.attrib.get("x")))
                                    right_y.append(
                                        float(point.attrib.get("y")))
                                right_xs.append(right_x)
                                right_ys.append(right_y)
                    if lane.tag == "reference_line":
                        reference_x = []
                        reference_y = []
                        for point in lane:
                            reference_x.append(float(point.attrib.get("x")))
                            reference_y.append(float(point.attrib.get("y")))
                        reference_xs.append(reference_x)
                        reference_ys.append(reference_y)

    return [reference_xs, reference_ys], [left_xs, left_ys], [right_xs, right_ys]


if __name__ == "__main__":
    print("display.")
    ret = ParseMapFile("../build/oxrd.xml")
    reference = ret[0]
    left = ret[1]
    right = ret[2]

    for index in range(0, len(reference[0])):
        plt.plot(reference[0][index], reference[1][index], color='blue')

    for index in range(0, len(left[0])):
        plt.plot(right[0][index], right[1][index], color='black')
        plt.plot(left[0][index], left[1][index], color='black')

    plt.show()
