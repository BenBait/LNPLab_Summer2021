import os

# assumes executable from get_ratios.cpp has already been made

# temps = ["25C", "35C", "45C"]
temps = ["25C"]


for T in temps:
    print(T)

    # should be the same on both sides
    path  = "../.."
    files = os.listdir(f"{path}/norm_movies/{T}")

    for F in files:
        movie   = f"{path}/norm_movies/{T}/{F}"
        command = f"./a.out {movie} {T} {F}"
        ## OUTFILE SAVED AT: 
        os.system(command)
