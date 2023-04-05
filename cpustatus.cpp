/* See LICENSE file for license details.
 * © 2023 Philipp Lindner <vredez at yahoo dot com>
 */

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

class CpuState {
public:
    CpuState(int core_count)
        : _core_count(core_count), _times(core_count * 2),
          _idles(core_count * 2)
    {
    }

    void parse(std::istream &input)
    {
        std::string l;
        int i = 0;
        while (i < _core_count && std::getline(input, l)) {
            if (l.rfind("cpu", 0) == 0) {
                if (l.size() >= 4u && l[3] == ' ') {
                    continue;
                }

                std::istringstream buf(l);
                std::vector<std::string> split{
                    std::istream_iterator<std::string>(buf), {}};

                _times[i] = 0;
                for (int cell = 1; cell < static_cast<int>(split.size());
                     ++cell) {
                    const int val = std::stoi(split[cell]);
                    _times[i] += val;
                    if (cell == 4) {
                        _idles[i] = val;
                    }
                }

                ++i;
            }
            else if (l.rfind("intr", 0) == 0) {
                return;
            }
        }
    }

    void print_usage(const CpuState &old, int symbol_count,
                     char *const *symbols) const
    {
        if (_core_count != old._core_count) {
            return;
        }

        for (int i = 0; i < _core_count; ++i) {
            const int delta = _times[i] - old._times[i];
            const int idle = _idles[i] - old._idles[i];
            const float usage =
                idle ? static_cast<float>(delta - idle) / idle : 1.0f;

            if (!symbol_count) {
                printf("%6.2f ", 100.0f * usage);
            }
            else {
                const int symbol_index = std::min(
                    symbol_count - 1,
                    static_cast<int>(std::floor(usage * symbol_count)));
                printf("%s ", symbols[symbol_index]);
            }
        }
        printf("\n");
    }

private:
    int _core_count;
    std::vector<int> _times;
    std::vector<int> _idles;
};

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "Usage: OLD_PROC_STAT | cpustatus CORECOUNT "
                        "[GLYPH0 GLYPH1 ...]\n");
        return 1;
    }

    const int core_count = std::stoi(argv[1]);

    CpuState old_state(core_count);
    old_state.parse(std::cin);

    std::fstream f;
    f.open("/proc/stat", std::ios::in);
    if (!f.is_open()) {
        fprintf(stderr, "Failed to read /proc/stat\n");
        return 1;
    }

    CpuState state(core_count);
    state.parse(f);
    f.close();

    state.print_usage(old_state, argc - 2, argv + 2);

    return 0;
}
