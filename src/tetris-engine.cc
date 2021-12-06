#include "tetris-engine.h"

#include <algorithm>

namespace cli_tetris::engine {

static constexpr int block_shape_i[2][4][2] = {
    {{0, 0}, {0, -1}, {0, 1}, {0, 2}},  // laying I
    {{0, 0}, {-1, 0}, {1, 0}, {2, 0}}};

static constexpr int block_shape_j[4][4][2] = {
    {{0, 0}, {0, -1}, {0, 1}, {1, 1}},
    {{0, 0}, {1, -1}, {1, 0}, {-1, 0}},
    {{0, 0}, {-1, -1}, {0, -1}, {0, -1}},
    {{0, 0}, {-1, 0}, {1, 0}, {-1, 1}}};

static constexpr int block_shape_l[4][4][2] = {
    {{0, 0}, {0, -1}, {1, -1}, {0, 1}},
    {{0, 0}, {-1, 0}, {-1, -1}, {1, 0}},
    {{0, 0}, {0, 1}, {-1, 1}, {0, -1}},
    {{0, 0}, {1, 0}, {1, 1}, {-1, 0}}};

static constexpr int block_shape_t[4][4][2] = {
    {{0, 0}, {0, -1}, {0, 1}, {1, 0}},
    {{0, 0}, {0, -1}, {-1, 0}, {1, 0}},
    {{0, 0}, {0, -1}, {-1, 0}, {0, 1}},
    {{0, 0}, {-1, 0}, {1, 0}, {0, 1}}};

static constexpr int block_shape_o[1][4][2] = {
    {{0, 0}, {0, 1}, {1, 0}, {1, 1}}};

static constexpr int block_shape_z[2][4][2] = {
    {{0, 0}, {0, -1}, {1, 0}, {1, 1}},
    {{0, 0}, {-1, 0}, {0, -1}, {1, -1}}};

static constexpr int block_shape_s[2][4][2] = {
    {{0, 0}, {1, -1}, {1, 0}, {0, 1}},
    {{0, 0}, {-1, -1}, {0, -1}, {1, 0}}};

constexpr object::YX TetrisEngine::kBlockStartPostion_;

TetrisEngine::TetrisEngine() {
    // Board Initialize
    for (auto line = board_.begin(); line != board_.end(); ++line) {
        for (auto column = (*line).begin(); column != (*line).end(); ++column) {
            *column = 0;  // kNothing
        }
    }

    previous_block_.pos.fill(object::YX(0, 0));
}

TetrisEngine::~TetrisEngine() {}
using TetrisBlock = struct TetrisBlock;
const decltype(TetrisEngine::board_)* TetrisEngine::getTetrisBoard() const {
    return &board_;
}
void TetrisEngine::SetBlockProperty(TetrisBlock* block, const int& random_number_of_4, const int& random_number_of_7) {
    switch (random_number_of_7) {
        case 1:
            block->type = BlockType::I;
            break;
        case 2:
            block->type = BlockType::J;
            break;
        case 3:
            block->type = BlockType::L;
            break;
        case 4:
            block->type = BlockType::T;
            break;
        case 5:
            block->type = BlockType::O;
            break;
        case 6:
            block->type = BlockType::Z;
            break;
        case 7:
            block->type = BlockType::S;
            break;
    }
    switch (block->type) {
        case BlockType::I:
        case BlockType::Z:
        case BlockType::S:
            if (random_number_of_4 == 1 || random_number_of_4 == 2)
                block->direction = 0;
            else if (random_number_of_4 == 3 || random_number_of_4 == 4)
                block->direction = 1;

            break;
        case BlockType::J:
        case BlockType::L:
        case BlockType::T:
            block->direction = random_number_of_4;
            break;
        case BlockType::O:
            block->direction = 0;
            break;
    }
}

bool TetrisEngine::SetBlockPostion(TetrisBlock* block) {
    int start_y = block->pos.at(0).y;
    int start_x = block->pos.at(0).x;
    int direction = block->direction;

    std::array<object::YX, 16> preview;

    switch (block->type) {
        case BlockType::I:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_i[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_i[direction][i][1] * 2);
            }
            break;
        case BlockType::J:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_j[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_j[direction][i][1] * 2);
            }
            break;
        case BlockType::L:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_l[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_l[direction][i][1] * 2);
            }
            break;
        case BlockType::T:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_t[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_t[direction][i][1] * 2);
            }
            break;
        case BlockType::O:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_o[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_o[direction][i][1] * 2);
            }
            break;
        case BlockType::Z:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_z[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_z[direction][i][1] * 2);
            }
            break;
        case BlockType::S:
            for (int i = 0; i <= 12; i += 4) {
                preview[i].y = start_y + (block_shape_s[direction][i][0] * 2);
                preview[i].x = start_x + (block_shape_s[direction][i][1] * 2);
            }
            break;
    }
    // 지정할 위치에 다른 Block 있는지 Check
    if (board_[preview[0].y][preview[0].x] != 0 ||
        board_[preview[4].y][preview[4].x] != 0 ||
        board_[preview[8].y][preview[8].x] != 0 ||
        board_[preview[12].y][preview[12].x] != 0) return false;

    for (int i = 0; i <= 12; i += 4) {
        preview[i + 1].y = preview[i].y;
        preview[i + 1].x = preview[i].x + 1;
        //지정할 위치에 다른 Block 있는지 Check
        if (board_[preview[i + 1].y][preview[i + 1].x] != 0) return false;

        preview[i + 2].y = preview[i].y + 1;
        preview[i + 2].x = preview[i].x;
        //지정할 위치에 다른 Block 있는지 Check
        if (board_[preview[i + 2].y][preview[i + 2].x] != 0) return false;

        preview[i + 3].y = preview[i].y + 1;
        preview[i + 3].x = preview[i].x + 1;
        //지정할 위치에 다른 Block 있는지 Check
        if (board_[preview[i + 3].y][preview[i + 3].x] != 0) return false;
    }

    block->pos = std::move(preview);
    return true;
}

void TetrisEngine::SetPositionToBoard(const TetrisBlock* const block, int value) {
    const std::array<object::YX, 16>& preview_pos = block->pos;
    for (auto itr = preview_pos.begin(); itr != preview_pos.end(); ++itr)
        board_[(*itr).y][(*itr).x] = value;
}

void TetrisEngine::PunchToBoard() {
    this->SetPositionToBoard(&previous_block_, 0);

    previous_block_ = (*current_block_);

    this->SetPositionToBoard(current_block_.get(), static_cast<int>(current_block_->type));
}

void TetrisEngine::CreateCurrentBlock(const int& random_number_of_4, const int& random_number_of_7) {
    if (!(1 <= random_number_of_4 && random_number_of_4 <= 4)) throw std::runtime_error(std::string("E013 : TetrisEngine::CreateCurrentBlock param is wrong"));
    if (!(1 <= random_number_of_7 && random_number_of_7 <= 7)) throw std::runtime_error(std::string("E013 : TetrisEngine::CreateCurrentBlock param is wrong"));

    if (current_block_ == nullptr)
        current_block_ = std::make_unique<TetrisBlock>(TetrisEngine::kBlockStartPostion_);

    this->SetBlockProperty(current_block_.get(), random_number_of_4, random_number_of_7);
    this->SetBlockPostion(current_block_.get());
}

void TetrisEngine::CreateNextBlock(const int& random_number_of_4, const int& random_number_of_7) {
    if (!(1 <= random_number_of_4 && random_number_of_4 <= 4)) throw std::runtime_error(std::string("E015 : TetrisEngine::CreateNextBlock param is wrong"));
    if (!(1 <= random_number_of_7 && random_number_of_7 <= 7)) throw std::runtime_error(std::string("E015 : TetrisEngine::CreateNextBlock param is wrong"));

    if (next_block_ == nullptr)
        next_block_ = std::make_unique<TetrisBlock>(TetrisEngine::kBlockStartPostion_);

    this->SetBlockProperty(next_block_.get(), random_number_of_4, random_number_of_7);
    this->SetBlockPostion(current_block_.get());
}

void TetrisEngine::MoveNextToCurrentBlock() {
    if (!(current_block_ == nullptr) && !(next_block_ == nullptr)) {
        current_block_.swap(next_block_);
        next_block_.release();
    }

    this->PunchToBoard();
}

const std::array<object::YX, 16> TetrisEngine::getNextBlockShape() const {
    std::array<object::YX, 16> next = next_block_->pos;

    int basis_y = next[0].y;
    int basis_x = next[0].x;
    for (auto itr = next.begin(); itr != next.end(); ++itr) {
        (*itr).y -= basis_y;
        (*itr).x -= basis_x;
    }

    return next;
}
bool TetrisEngine::RotateCurrentBlock() {
    if (!(current_block_ == nullptr)) {
        int original_direction = current_block_->direction;
        int direction = original_direction;

        switch (current_block_->type) {
            case BlockType::I:
            case BlockType::Z:
            case BlockType::S:

                direction = (direction + 1) % 2;

                break;
            case BlockType::J:
            case BlockType::L:
            case BlockType::T:
                direction = (direction + 1) % 4;
                break;
            case BlockType::O:
                direction = 0;
                break;
        }

        current_block_->direction = direction;

        if (this->SetBlockPostion(current_block_.get()))
            PunchToBoard();
        else {
            current_block_->direction = original_direction;
            return false;
        }

        return true;
    }

    return false;
}
bool TetrisEngine::FallCurrentBlock() {
    current_block_->pos.at(0).y++;

    if (this->SetBlockPostion(current_block_.get()))
        PunchToBoard();
    else {
        current_block_->pos.at(0).y--;
        return false;
    }

    return true;
}

bool TetrisEngine::MovingCurrentBlock(Move where) {
    int offset_y = 0;
    int offset_x = 0;
    switch (where) {
        case Move::kDown:
            offset_y = 1;
            break;
        case Move::kLeft:
            offset_x = -1;
            break;
        case Move::kRight:
            offset_x = 1;
            break;
        case Move::kUP:
            // Nothing
            break;
    }
    std::array<object::YX, 16>& preview = current_block_->pos;
    std::transform(preview.begin(), preview.end(), preview.begin(), [&offset_y, &offset_x](object::YX& i) {
        i.y = offset_y;
        i.x = offset_x;
        return i;
    });

    if (preview.end() != std::find_if(preview.begin(), preview.end(), [this](const object::YX& i) -> bool {
            if (i.y >= this->board_.size() || i.y < 0 || i.x >= this->board_.at(0).size() || i.x < 0) return true;
            if (this->board_[i.y][i.x] != 0) return true;
            return false;
        })) {
        offset_y *= -1;
        offset_x *= -1;
        std::transform(preview.begin(), preview.end(), preview.begin(), [&offset_y, &offset_x](object::YX& i) {
            i.y = offset_y;
            i.x = offset_x;
            return i;
        });

        return false;
    }

    this->PunchToBoard();
    return true;
}

bool TetrisEngine::IsNextBlockExist() const {
    if (next_block_ == nullptr)
        return false;
    else
        return true;
}

void TetrisEngine::FixedCurrentBlockToBoard() {
    this->PunchToBoard();
}
void TetrisEngine::DeleteCompleteLines() {
    auto& shape = current_block_->pos;

    for (auto target = shape.cbegin(); target != shape.cend(); ++target) {
        if (board_.at((*target).y).cend() != std::find_if(board_.at((*target).y).cbegin() + 1, board_.at((*target).y).cend(),
                                                          [](const int& i) -> bool {
                                                              if (i != 0)
                                                                  return true;
                                                              else
                                                                  return false;
                                                          })) {
            continue;
        } else {
            this->ArrangeBoard((*target).y);
        }
    }
}
void TetrisEngine::ArrangeBoard(const int& line) {
    for (int n = line; n != 0; n--) {
        std::copy(board_.at(line - 1).cbegin(), board_.at(line - 1).cend(),
                  board_.at(line).begin());
    }
}

}  // namespace cli_tetris::engine