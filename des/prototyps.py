#!/usr/bin/python3
# -*- coding: UTF-8 -*-
# Данная программа считывает файл, переданный вторым аргументом и формирует
# на основе этого файла прототипы функций, которые встречаются в этом файле.
# Эти прототипы она записывате в конец файла .h, имя которого пердается вторым
# парараметром. Далее все функции файла программа рапределяет по отдельным
# файлам, по пять функций на файл.

import sys


class Prototype:
    prototypes = []
    functions = []
    name_files = []
    func = ""
    head = ""

    def __init__(self):
        if len(sys.argv) != 3:
            print("Первый аргумент - имя файла в которм собираются прототипы.")
            print("Второй аргумент - куда будут записаны прототипы")
            exit()
        try:
            f = open(sys.argv[1], 'r')
            for line in f:
                self.func += line
                if line[0].isalpha():
                    self.prototypes.append((line[:-1] + ";"))
                    self.name_files.append(self.name_function(line))
                if line[0] == '}':
                    self.functions.append(self.func)
                    self.func = ""
            f.close()
        except IOError:
            print("Неверное имя файла!")
            exit(0)
        f.close()
        self.head = sys.argv[2]
        self.write_prototyps()
        self.write_funtions()

    def name_function(self, line):
        name = ""
        name = line[line.find("\t"):line.find("(")]
        name = name.lstrip(' \t*')
        name += ".c"
        return name

    def write_prototyps(self):
        try:
            out = open(self.head, "a")
            print("\nВ файл " + self.head + " добавленны следующие прототипы:")
            for i in range(len(self.name_files)):
                if i % 5 == 0:
                    out.write("/*\n** File " + self.name_files[i] + "\n*/\n")
                print(self.prototypes[i])
                out.write(self.prototypes[i] + "\n")
            out.close()
        except IOError:
            print("Ошибка открытия файла!")
            exit(0)

    def write_funtions(self):
        try:
            print("\nСозданы следующие файлы:")
            for i in range(len(self.name_files)):
                if i % 5 == 0:
                    print(self.name_files[i])
                    out = open(self.name_files[i], "w")
                    out.write("\n#include \"" + self.head + "\"\n\n")
                out.write(self.functions[i])
            out.close()
        except IOError:
            print("Ошибка открытия файла!")
            exit(0)


if __name__ == "__main__":
    prot = Prototype()
