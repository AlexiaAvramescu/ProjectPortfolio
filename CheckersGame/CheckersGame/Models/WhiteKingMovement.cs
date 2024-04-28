using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.Models
{
    internal class WhiteKingMovement : IMovement
    {
        public Collection<int> GetPosibleMovements(ObservableCollection<Piece> board, Position fromPos)
        {
            Collection<int> possiblePos = new Collection<int>();
            int fromIndex = GetIndex(fromPos.Line, fromPos.Column);

            if (fromIndex < 0 || fromIndex >= board.Count)
                return possiblePos;

            int[] forwardRowOffsets = { -1, 1 };
            int[] columnOffsets = { -1, 1 };

            foreach (int forwardRowOffset in forwardRowOffsets)
            {
                foreach (int columnOffset in columnOffsets)
                {
                    int futurePosIndex = GetIndex(fromPos.Line + forwardRowOffset, fromPos.Column + columnOffset);
                    if (futurePosIndex < 0 || futurePosIndex >= board.Count)
                        continue;
                    if (board[futurePosIndex].IsNull) possiblePos.Add(futurePosIndex);
                    if (board[futurePosIndex].Color == EColor.Black)
                    {
                        int capturePosIndex = GetIndex(fromPos.Line + forwardRowOffset * 2, fromPos.Column + columnOffset * 2);
                        if (capturePosIndex < 0 || capturePosIndex >= board.Count)
                            continue;
                        if (board[capturePosIndex].IsNull)
                            possiblePos.Add(capturePosIndex);
                    }
                }
            }

            return possiblePos;
        }

        private int GetIndex(int line, int column)
        {
            return (line * 8) + column;
        }

        private Position GetPosition(int index)
        {
            return new Position(index / 8, index % 8);
        }
    }
}
