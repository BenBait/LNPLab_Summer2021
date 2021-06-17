import os

# assumes executable from get_part_ratios.cpp has already been made

temps = ["25C"]

for T in temps:
    print(T)

    # should be the same on both sides
    # files = os.listdir(f"../norm_movies/{T}")
    path = "../.."
    part_locs = f"{path}/part_data/{T}/L"
    spools = os.listdir(part_locs)

    for F in spools:
        l_locs  = part_locs + f"/{F}"
        r_locs  = f"{path}/part_data/{T}/R/{F}"

        locs_dir  = os.listdir(l_locs)
        num_parts = len(locs_dir)

        locs = ""

        for i in range(0, num_parts):
            locs = locs + f"{l_locs}/{locs_dir[i]} {r_locs}/{locs_dir[i]} "

        movie    = f"{path}/norm_movies/{T}/{F}.asc"
        command  = f"./a.out {movie} {T} {F} {num_parts} " + locs

        os.system(command)
