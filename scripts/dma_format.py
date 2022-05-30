import csv
from pprint import pprint
import pandas as pd
import re

a = set()
lst = []
strpath = 'C:\\Users\\daniil\\Documents\\code\\stm\\workspace\\lib\\mcu_defs\\STM32F411CE\\dma_map.csv'
with open(strpath) as csvfile:
    spamreader = csv.reader(csvfile, delimiter=';', quotechar='"')
    for row in spamreader:
        ooo = []
        for elem in row:
            ooo.append(elem)
            l = elem.split(':')
            for item in l:
                a.add(item)
        lst.append(ooo)
        #print(', '.join(row))
a.remove("-")


def find_label(table, label):
    idx = []
    for i in range(table.shape[0]):
        for j in range(table.shape[1]):
            if(label in table[j][i]):
                idx.append((i, j))
    return idx


ii = [x for x in a]
ii.sort()
pprint(ii)

table = pd.read_csv(strpath, sep=";", header=None)
for label in ii:
    print(label, find_label(table, label))
print(table.shape)

with open('./libs/superlib/inc/dma_format.hpp.temp', 'tw') as cpp:
    cpp.writelines("""
#include <array>
#include <cstdint>

namespace mcu {
namespace dma {
namespace routing {
struct cfg {
    std::uint8_t dma;
    std::uint8_t stream;
    std::uint8_t channel;
};
template <size_t N>  using routes = std::array<cfg, N>;
enum class source { 
    """)
    for labb in ii:
        cpp.write("""{},\n""".format(labb.lower()))
    cpp.write("};\n")
    for label in ii:
        u = find_label(table, label)
        cpp.write("""constexpr routes<{}> {}{}""".format(
            len(u), label.lower(), '{{'))
        for iidx, t in enumerate(u):
            cpp.write("{"+"{}, {}, {}".format(t[1]//8+1, t[1] % 8, t[0])+"}")
            if iidx < len(u)-1:
                cpp.write(""",""")
        cpp.write("""}};\n""")
    cpp.writelines("""
} // namespace routing
} // namespace dma
} // namespace mcu
    """)
