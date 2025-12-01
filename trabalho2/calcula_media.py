#!/usr/bin/env python3
import subprocess, os, sys

def run_program(src, runs=10):
    bin_name = os.path.splitext(os.path.basename(src))[0]
    subprocess.run(["gcc", "-fopenmp", "-o", bin_name, src])
    for i in range(1, runs + 1):
        print(f"Execução {i}:")
        subprocess.run([f"./{bin_name}"])

def calcula_media(fname):
    with open(fname) as f:
        valores = [float(l.strip()) for l in f if l.strip()]
    return sum(valores) / len(valores), len(valores)

if __name__ == "__main__":
    c_file = sys.argv[1]
    txt_file = sys.argv[2]
    run_program(c_file)          # 10 execuções padrão
    media, n = calcula_media(txt_file)
    print(f"Média: {media} (n={n})")