import json

filenames = [
"011011_bottom_horizontal_border_8.png",
"010110_top_left_border_7.png",
"000001_bottom_right_border_11.png",
"101101_right_border_4.png",
"001101_top_right_border_6.png",
"101110_horizontal_border_2.png",
"001110_top_horizontal_border_5.png",
"110101_central_0.png",
"010000_bottom_left_border_9.png",
"110110_left_border_3.png",
"010001_bottom_border_10.png",
"010101_top_border_1.png"
]

walls = []

file = [filename for filename in filenames if "bottom" not in filename]
lower_files_score = []

for x in range(64):
    entry = {}
    value = bin(x)[2:].zfill(6)
    upper_files = [filename for filename in filenames if "bottom" not in filename and filename.split("_")[0][0] == value[0] and filename.split("_")[0][1] == value[1] and filename.split("_")[0][5] == value[5]]
    entry["upper"] = int(upper_files[0].split("_")[len(upper_files[0].split("_")) - 1].split(".")[0])

    #lower_files = [filename for filename in filenames if "top" not in filename and (1 if int(value[1]) == int(value[2] and int(value[1]) == int(filename.split("_")[0][1])) else 0) and (int(filename.split("_")[0][3]) == int(value[3])) and (1 if int(value[5]) == int(value[4]) and int(value[5]) == int(filename.split("_")[0][5]) else )]
    lower_files = [filename for filename in filenames if "top" not in filename and ((int(value[1]) or int(filename.split("_")[0][1]) or int(value[2]) or int(filename.split("_")[0][2])) and ((not int(value[1]) or ((int(value[1]) and not int(value[2]))) and int(value[4]) and int(value[5]) and "bottom_right" in filename) and ((not int(value[5]) or (int(value[5]) and not int(value[4]))) and int(value[1]) and int(value[2]) and "bottom_left" in filename) or (int(filename.split("_")[0][3]) == int(value[3])) and (int(value[4]) or int(filename.split("_")[0][4]) or int(value[5]) or int(filename.split("_")[0][5])))) or filename.split("_")[0] == value]
    #lower_files = [filename for filename in filenames if "top" not in filename]

    score_obj = {}
    if int(value[1]) == 0 or int(value[2]) == 0 or int(value[4]) == 0 or int(value[5]) == 0:
        lower_files = [filename for filename in lower_files if "central" not in filename]

    if int(value[1]) == 1 and int(value[2]) == 0 and int(value[4]) == 0 and int(value[5]) == 1:
        lower_files = [filename for filename in lower_files if "right" not in filename and "left" not in filename]

    for file in lower_files:
        score = 0
        if int(value[1]) == int(value[2]) and int(value[1]) == int(file.split("_")[0][1]):
            score += 2
        elif int(value[1]) != int(value[2]) and int(value[1]) == int(file.split("_")[0][1]):
            score += 1
        if int(value[4]) == int(value[5]) and int(value[5]) == int(file.split("_")[0][5]):
            score += 2
        elif int(value[4]) != int(value[5]) and int(value[5]) == int(file.split("_")[0][5]):
            score += 1
            #Encajar el 16 (bottom left border) + casos raros mediante los nombres
        if int(value[5]) == 0 and int(value[1]) == 0 and "bottom_horizontal_border" in file:
            score += 1
        if (int(value[5]) != 0 or int(value[1]) != 0) and "bottom_horizontal_border" in file:
            score -= 1
        if (int(value[3]) == 1 and int(value[5]) == 1) and "left_border" in file:
            score += 2
        if (int(value[3]) == 1 and int(value[5]) == 1) and "right_border" in file:
            score -= 2
        if (int(value[1]) == 1 and int(value[3]) == 1) and "right_border" in file:
            score += 2
        if (int(value[1]) == 1 and int(value[3]) == 1) and "left_border" in file:
            score -= 2
        if int(value[1]) == 0 and "right_border" in file:
            score += 2
        if int(value[5]) == 0 and "left_border" in file:
            score += 2
        if (int(value[1]) == 0 or int(value[2]) == 0) and (int(value[4]) == 0 or int(value[5]) == 0) and "horizontal_border" in file:
            score += 2
        if (int(value[1]) == 0 or int(value[2]) == 0) and (int(value[4]) == 0 or int(value[5]) == 0) and "horizontal_border" not in file:
            score -= 2
        score_obj[file] = score

    max_value = max(score_obj.values())
    max_keys = [k for k, v in score_obj.items() if v == max_value]
    if ("bottom" in max_keys[0] and "bottom_border" not in max_keys[0]) or (int(value[5]) != int(value[1]) and "bottom_horizontal" in max_keys[0]):
        if int(value[1]) == 0 and int(value[5]) != 0:
            max_keys[0] = "000001_bottom_right_border_11.png"
        elif int(value[5]) == 0 and int(value[1]) != 0:
            max_keys[0] = "010000_bottom_left_border_9.png"
        elif int(value[5]) == 1 and int(value[1]) == 1:
            max_keys[0] = "010001_bottom_border_10.png"
        elif int(value[1]) == 0 and int(value[5]) == 0:
            max_keys[0] = "011011_bottom_horizontal_border_8.png"


    lower_files_score.append(score_obj)
    entry["lower"] = int(max_keys[0].split("_")[len(max_keys[0].split("_")) - 1].split(".")[0])
    walls.append(entry);

print("\n")
print(json.dumps(lower_files_score, indent=4))
print(json.dumps(walls, indent=4))

with open('resources/walls.json', 'w') as outfile:
    json.dump(walls, outfile, indent=4)
