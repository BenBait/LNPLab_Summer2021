import matplotlib.pyplot as plt
import numpy as np
# import get_var_each_mp as var
plt.rcParams.update({'font.size': 9})

temps   = ["25C"]
RANGE     = np.array([0.04, 0.02])
NUM_SIZES = 50

for T in temps:

    tot_sectors = 20
    m_s = 19

    file = open(f"./{T}_parts_err_space_and_frames.txt")

    mean_var = np.genfromtxt(file)
    mean_var = mean_var[:-((tot_sectors - m_s)*NUM_SIZES)]
    mean_var = np.split(mean_var, m_s)

    time_needed  = np.zeros((2, m_s))
    first        = np.array([1, 1, 1])

    for i in range(0, m_s):
        for j in range(0, NUM_SIZES):

            for k in range(0, 2):
                if mean_var[i][j] < RANGE[k]:
                    if first[k] == 1:
                        time_needed[k][i] = j
                        first[k] = 0 #False
                else:
                    first[k] = 1 #True

    print("TIME NEEDED")
    print(time_needed)
    size = np.zeros(2)

    # we know by observing above time_needed ^^
    sector_0_3 = 4
    sector_0_2 = 11

    new_time3 = time_needed[0][time_needed[0] != 0]
    new_time2 = time_needed[1][time_needed[1] != 0]
    print(new_time3)
    print(new_time2)

    size_3 = len(new_time3)
    size_2 = len(new_time2)

    x = np.arange(0, size_2 + 4, 1)
    x_3 = np.arange(sector_0_3, size_3 + sector_0_3, 1)
    x_2 = np.arange(sector_0_2, size_2 + sector_0_2, 1)

    fig = plt.figure(figsize=(3.5, 3.5))
    ax = fig.add_subplot(111)
    
    ax.plot(x_3, new_time3, label = f"Variation < 0.03")
    ax.plot(x_2, new_time2, "g--", label = f"Variation < 0.02")

    # locs, labels = plt.xticks()
    # plt.xticks(x, x_labels)

    # y_labels = ['2', '4', '6', '8', '10', '12', '14', '16', '18', '20', '22',
                # '24', '26', '28', '30', '32', '34', '36', '38', '40', '42',
                # '44', '46', '48', '50']

    # y = np.arange(0, 26, 1)
    # locs, labels = plt.yticks()
    # plt.yticks(y, y_labels, fontsize = 6)

    ax.set(ylabel='Frames',
           xlabel='Particles Included')

    degree_sign = u"\N{DEGREE SIGN}"
    fig_title = f'Frames Required (for mean error to be in range)\n'\
                + f'vs. Particles Included\n(Temp = {T})'

    plt.title(fig_title, y=1.05, fontsize=10)


    ax.legend(title='Variation Range', loc='upper left',
              shadow='True', prop={'size': 5})

    plt.tight_layout()

    plt.savefig(f'resulting_graphs/{T}_part_frames_req_vs_ms.png')
