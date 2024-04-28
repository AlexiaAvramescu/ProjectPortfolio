using Newtonsoft.Json;
using System;
using System.Collections.ObjectModel;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.Models
{
    public class Board : ISerializable
    {
        const int kLines = 8;
        const int kCollumns = 8;
        const int kNumberPiecces = 12;
        public ObservableCollection<Piece> Pieces { get; set; }

        public int BlackPiecesCount { get; set; }
        public int WhitePiecesCount { get; set; }
        [JsonConstructor]
        private Board() { }
        public Board(int dummy)
        {
            Initialize();
        }

        public void Initialize()
        {
            Pieces = new ObservableCollection<Piece>();
            for (int i = 0; i < kLines; i++)
            {
                for (int j = 0; j < kCollumns; j++)
                {
                    if (((i == 0 || i == 2) && j % 2 == 1) || i == 1 && j % 2 == 0)
                        Pieces.Add(new Piece(EColor.White, EType.Queen, i, j));
                    else if (i == 6 && j % 2 == 1 || (i == 7 || i == 5) && j % 2 == 0)
                        Pieces.Add(new Piece(EColor.Black, EType.Queen, i, j));
                    else
                        Pieces.Add(new Piece(i, j));
                }
            }

            BlackPiecesCount = kNumberPiecces;
            WhitePiecesCount = kNumberPiecces;
        }
        public bool MakeMove(Piece pieceToMove, Piece destination, bool firstMoveMade, ref bool lastMoveCapture)
        {
            if (Math.Abs(destination.Line - pieceToMove.Line) == 1 && !firstMoveMade)
            {
                pieceToMove.MoveTo(destination);
                pieceToMove.Clear();
                lastMoveCapture = false;
                return true;
            }
            else if (Math.Abs(destination.Line - pieceToMove.Line) == 2)
            {
                if (firstMoveMade && !lastMoveCapture)
                    return false;
                pieceToMove.MoveTo(destination);
                int capturedRow = (destination.Line + pieceToMove.Line) / 2;
                int capturedCol = (destination.Column + pieceToMove.Column) / 2;
                int capturedIndex = capturedRow * 8 + capturedCol;

                if (Pieces[capturedIndex].Color == EColor.White)
                    WhitePiecesCount--;
                else
                    BlackPiecesCount--;

                Pieces[capturedIndex].Clear();
                pieceToMove.Clear();
                if (!firstMoveMade)
                    lastMoveCapture = true;
                return true;
            }
            return false;
        }

        public void SetPieceMovement()
        {
            foreach (var piece in Pieces) 
            {
                piece.SetMovement();
            }
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue(nameof(Pieces), Pieces);
            info.AddValue(nameof(BlackPiecesCount), BlackPiecesCount);
            info.AddValue(nameof(WhitePiecesCount), WhitePiecesCount);
        }

    }
}
