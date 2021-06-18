import os

# assumes executable from get_part_ratios.cpp has already been made

# temps = ["25C"]
temps = ["25C", "35C", "45C"]

for T in temps:
    print(T)

    # should be the same on both sides
    path = "../.."
    part_locs = f"{path}/part_data/{T}/L"

    # gives coordinates of particles for each spool
    spools = os.listdir(part_locs)

    for F in spools:
        l_locs  = part_locs + f"/{F}"
        r_locs  = f"{path}/part_data/{T}/R/{F}"

        # only need left locations, bc right side is the same particles
        locs_dir  = os.listdir(l_locs)
        # number of particles in curr directory
        num_parts = len(locs_dir)

        locs = ""

        for i in range(0, num_parts):
            locs = locs + f"{l_locs}/{locs_dir[i]} {r_locs}/{locs_dir[i]} "

        movie    = f"{path}/norm_movies/{T}/{F}.asc"
        command  = f"./a.out {movie} {T} {F} {num_parts} " + locs

        os.system(command)
