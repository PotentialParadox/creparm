from gene import Gene

gene_0 = Gene("first_run")
gene_1 = Gene("second_run")
gene_2 = Gene("third_run")
init = Gene("initial")

# differences = []
# for i, j, k in zip(gene_0.p_floats,
#                    gene_1.p_floats,
#                    gene_2.p_floats):
#     average_value = (i + j + k) / 3
#     i_diff = abs(i - average_value)
#     j_diff = abs(j - average_value)
#     k_diff = abs(k - average_value)
#     average_diff = (i_diff + j_diff + k_diff)/(3 * average_value)
#     differences.append(average_diff)
# print(sum(differences)/len(differences))

differences = []
for i, j, k, l in zip(gene_0.p_floats,
                      gene_1.p_floats,
                      gene_2.p_floats,
                      init.p_floats):
    average_value = (i + j + k) / 3
    init_diff = abs(l - average_value) / average_value
    differences.append(init_diff)
print(sum(differences)/len(differences))
