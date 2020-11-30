import json

filenames = [
"000000_bottom_horizontal_border_0.png",
"010110_top_left_border_1.png",
"000001_bottom_right_border_2.png",
"101101_right_border_3.png",
"001101_top_right_border_4.png",
"101110_horizontal_border_5.png",
"001110_top_horizontal_border_6.png",
"110101_central_7.png",
"010000_bottom_left_border_8.png",
"110110_left_border_9.png",
"010001_bottom_border_10.png",
"010101_top_border_11.png"
]

walls = {}

file = [filename for filename in filenames if "bottom" not in filename]
lower_files_score = []

for x in range(64):
    entry = {}
    value = bin(x)[2:].zfill(6)
    upper_files = [filename for filename in filenames if "bottom" not in filename and filename.split("_")[0][0] == value[0] and filename.split("_")[0][1] == value[1] and filename.split("_")[0][5] == value[5]]
    entry["upper"] = upper_files


    #lower_files = [filename for filename in filenames if "top" not in filename and (1 if int(value[1]) == int(value[2] and int(value[1]) == int(filename.split("_")[0][1])) else 0) and (int(filename.split("_")[0][3]) == int(value[3])) and (1 if int(value[5]) == int(value[4]) and int(value[5]) == int(filename.split("_")[0][5]) else )]
    lower_files = [filename for filename in filenames if "top" not in filename and (int(value[1]) or int(filename.split("_")[0][1]) or int(value[2]) or int(filename.split("_")[0][2])) and (int(filename.split("_")[0][3]) == int(value[3])) and (int(value[4]) or int(filename.split("_")[0][4]) or int(value[5]) or int(filename.split("_")[0][5]))]
    #print(lower_files)
    score_obj = {}
    if int(value[1]) == 0 or int(value[2]) == 0 or int(value[4]) == 0 or int(value[5]) == 0:
        lower_files = [filename for filename in lower_files if "central" not in filename]
    if int(value[1]) == 1 and int(value[2]) == 0 and int(value[4]) == 0 and int(value[5]) == 1:
        lower_files = [filename for filename in lower_files if "right" not in filename and "left" not in filename]
    for file in lower_files:
        score = 0
        if int(value[1]) == int(value[2]) and int(value[1]) == int(file.split("_")[0][1]):
            score += 2 #Faltaría hacer una última comparación en caso de empate
            #Puntuar 2 aqui. Si 1 y 2 no son iguales, puntuar 1 y probar lo mismo para left
        elif int(value[1]) != int(value[2]) and int(value[1]) == int(file.split("_")[0][1]):
            score += 1
        if int(value[4]) == int(value[5]) and int(value[5]) == int(file.split("_")[0][5]):
            score += 2
        elif int(value[4]) != int(value[5]) and int(value[5]) == int(file.split("_")[0][5]):
            score += 1
        score_obj[file] = score

    max_value = max(score_obj.values())  # maximum value
    max_keys = [k for k, v in score_obj.items() if v == max_value]
    print(str(x) + ': ' + str(max_keys))
    lower_files_score.append(score_obj)
    entry["lower"] = lower_files


    #print(upper_files)
    #print(lower_files)
    # print(int("1") == int("0") and (int("1") == int("1")))
    # print((int(lower_file[0].split("_")[0][1]) == int(value[1])) and (int(lower_file[0].split("_")[0][2]) or int(value[2])) and (int(lower_file[0].split("_")[0][3]) == int(value[3])) and (int(lower_file[0].split("_")[0][4]) or int(value[4])) and (int(lower_file[0].split("_")[0][5]) == int(value[5])))
    # print(int(lower_file[0].split("_")[0][1]))
    # print(int(lower_file[0].split("_")[0][2]))
    # print(int(lower_file[0].split("_")[0][3]))
    # print(int(lower_file[0].split("_")[0][4]))
    # print(int(lower_file[0].split("_")[0][5]))
    # print(int(value[1]))
    # print(int(value[2]))
    # print(int(value[3]))
    # print(int(value[4]))
    # print(int(value[5]))
    #print(lower_file[0].split("_")[0])
    #print(value)

    walls[x] = entry;

print("\n")
print(json.dumps(lower_files_score, indent=4))
print(json.dumps(walls, indent=4))
