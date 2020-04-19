#include <cstdio>
#include <memory>
#include <random>
#include <thread>
#include <chrono>

namespace {

class LifeGame {
  public:
    explicit LifeGame(int row, int column) : row_(row), column_(column) {
        field_.reset(new bool[row * column]);

        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<> int_dis(0, 9);

        for (int i = 0; i < row; ++i) {
            for (int j = 0; j < column; ++j) {
                field_[i * row + j] = int_dis(generator) == 0;
            }
        }
    }

    void Render() const {
        printf("\033[0;0H"); // clear

        for (int i = 0; i < row_; ++i) {
            for (int j = 0; j < column_; ++j) {
                if (field_[i * row_ + j]) {
                    printf("█");
                } else {
                    putc(' ', stdout);
                }
            }
            putc('\n', stdout);
        }
    }

    void Update() {
        bool *updated_field = new bool[row_ * column_];

        for (int i = 0; i < row_; ++i) {
            for (int j = 0; j < column_; ++j) {
                updated_field[i * row_ + j] = IsAlive(i, j);
            }
        }

        field_.reset(updated_field);
    }

  private:
    bool IsAlive(int row, int column) {
        int neighbor_lives = 0;
        for (int i = -1; i <= 1; ++i) {
            for (int j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0) {
                    continue;
                }

                neighbor_lives += CellState(row + i, column + j);
            }
        }

        const size_t index = row * row_ + column;
        if (field_[index]) {
            return neighbor_lives == 2 || neighbor_lives == 3;
        }

        return neighbor_lives == 3;
    }

    bool CellState(int row, int column) {
        if (row < 0 || row >= row_) {
            return false;
        }
        if (column < 0 || column >= column_) {
            return false;
        }

        return field_[row * row_ + column];
    }

    int row_;
    int column_;
    std::unique_ptr<bool[]> field_;
};

} // namespace

int main() {
    LifeGame life_game(30, 90);
    life_game.Render();

    for (size_t i = 0; i < 10000; ++i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));

        life_game.Update();
        life_game.Render();
    }

    return 0;
}
