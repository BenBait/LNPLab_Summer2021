import os

# assumes executable from get_norm.cpp has already been made

temps = ["25C", "35C", "45C"]

for T in temps:
    print(T)

    path  = ".." # change this to wherever you have the files
    instr = f"{path}/movies_SP2021/instrumental_err/spool3.asc"
    oxa   = f"{path}/movies_SP2021/oxazine/{T}spool1.asc"
    gel   = f"{path}/movies_SP2021/pure_gel/{T}.asc"

    files = os.listdir(f"../movies_SP2021/{T}")

    for F in files:
        movie   = f"../movies_SP2021/{T}/{F}"
        command = f"./a.out {movie} {oxa} {instr} {gel} {F}"
        ## OUTFILE SAVED AT: ../norm_movies/{T}
        ## be sure to make this directory and a subdirectory for each temp
        os.system(command)
