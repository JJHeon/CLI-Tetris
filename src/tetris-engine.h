/** Note
 * tetris-engine
 *
 * tetris logic 객체
 *
 */

#ifndef CLI_TETRIS_TETRIS_ENGINE_H_
#define CLI_TETRIS_TETRIS_ENGINE_H_

#include <array>
#include <memory>

#include "object-defined.h"

namespace cli_tetris::engine {

using Move = enum class Move { kNothing = 0,
                               kUP,
                               kDown,
                               kLeft,
                               kRight };

using BlockType = enum class BlockType {
    kNothing = 0,
    I = 1,
    J = 2,
    L = 3,
    T = 4,
    O = 5,
    Z = 6,
    S = 7,
    kCurrentBase = 10,
    kCurrentI = 11,
    kCurrentJ = 12,
    kCurrentL = 13,
    kCurrentT = 14,
    kCurrentO = 15,
    kCurrentZ = 16,
    kCurrentS = 17
};

class TetrisEngine {
   private:
    using TetrisBlock = struct TetrisBlock {
        std::array<object::YX, 4> pos;
        BlockType type;
        int direction;
        constexpr TetrisBlock(const object::YX& start_pos)
            : type(BlockType::kNothing), direction(0) {
            pos.at(0) = start_pos;
        }
        constexpr TetrisBlock() : type(BlockType::kNothing), direction(0) {
            pos.fill(object::YX(0, 0));
        }
        TetrisBlock& operator=(const TetrisBlock& target) {
            pos = target.pos;
            type = target.type;
            direction = target.direction;

            return *this;
        }
    };
    constexpr static object::YX kBlockStartPostion_ = object::YX(20, 5);

    std::vector<std::vector<int>> board_;

    TetrisBlock previous_block_;
    std::unique_ptr<TetrisBlock> current_block_;
    std::unique_ptr<TetrisBlock> next_block_;

   private:
    void SetBlockProperty(TetrisBlock* block, const int& random_number_of_4, const int& random_number_of_7);
    bool SetBlockPostion(TetrisBlock* block);
    void SetPositionToBoard(const TetrisBlock* const pos, int value);
    void UpdateCuttenrBlockDirection(TetrisBlock* block);
    void PunchToBoard(BlockType what_value);
    void ArrangeBoard(const int& line);
    bool IsPlacedBlock(const decltype(TetrisBlock::pos)& block_pos);
    bool IsCanFallBlock(const decltype(TetrisBlock::pos)& block_pos);
    BlockType ConvertCurrentBlockType(const BlockType& type);
    void ClearPreviousBlock();

   public:
    TetrisEngine();
    ~TetrisEngine();

    /**
     * @param
     * random_number_of_4 : Dicide direction
     * random_number_of_7 : Dicide blocktypes
     */
    void CreateCurrentBlock(const int& random_number_of_4, const int& random_number_of_7);
    /**
     * @param
     * random_number_of_4 : Dicide direction
     * random_number_of_7 : Dicide blocktypes
     */
    void CreateNextBlock(const int& random_number_of_4, const int& random_number_of_7);
    bool MoveNextToCurrentBlock();

    const decltype(board_)* getTetrisBoard() const;
    const decltype(TetrisBlock::pos) getNextBlockShape() const;
    bool RotateCurrentBlock();
    bool FallCurrentBlock();
    bool MovingCurrentBlock(Move where);
    bool IsNextBlockExist() const;
    void FixedCurrentBlockToBoard();
    void DeleteCompleteLines();
    // LATER:
    // bool IsCurrentBlockExist() const;
    // getCurrentBlockType();
    // getFinishedLineNumbers();
};

}  // namespace cli_tetris::engine

#endif  // CLI_TETRIS_TETRIS_ENGINE_H_