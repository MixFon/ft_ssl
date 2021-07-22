
file_name = input("Enter file name\n")

file = open(file_name, 'r')
data = ""
for line in file:
    data += line.replace("    ", "\t").strip(" ")

new_file = open("new_" + file_name, "w")
new_file.write(data)
