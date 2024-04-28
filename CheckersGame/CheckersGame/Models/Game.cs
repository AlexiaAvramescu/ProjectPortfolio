using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.Models
{
    public class Game : ISerializable, INotifyPropertyChanged
    {

        private EColor _currentPlayer;
        private bool _firstMoveMade;
        private Piece _selectedPiece;
        private bool _multipleJumps;
        private bool _isGameOver;
        private bool _notStarted;

        public event PropertyChangedEventHandler PropertyChanged;

        public bool NotStarted
        {
            get { return _notStarted; }
            set { _notStarted = value; OnPropertyChanged(nameof(NotStarted)); }
        }

        public bool FirstMoveMade { get; set; }
        private bool FirstMoveCapture { get; set; }

        public bool IsGameOver
        {
            get { return _isGameOver; }
            set { _isGameOver = value; OnPropertyChanged(nameof(IsGameOver)); }

        }
        public Piece SelectedPiece
        {
            get { return _selectedPiece; }
            set
            {
                _selectedPiece = value;
                OnPropertyChanged(nameof(SelectedPiece));
                OnPropertyChanged(nameof(Board.Pieces));
            }
        }
        public EColor CurrentPlayer
        {
            get { return _currentPlayer; }
            set
            {
                _currentPlayer = value;
                OnPropertyChanged(nameof(CurrentPlayer));
            }
        }
        public Board Board { get; set; }
        public bool MultipleJumps
        {
            get { return _multipleJumps; }
            set
            {
                _multipleJumps = value;
            }
        }

        public Game() { }
        public Game(bool multipleJumps)
        {
            Board = new Board(1);
            MultipleJumps = multipleJumps;
            CurrentPlayer = EColor.Black;
            SelectedPiece = new Piece(-1, -1);
            IsGameOver = false;
            NotStarted = true;
            FirstMoveMade = false;
            FirstMoveCapture = false;
        }

        public void OnPieceClicked(Piece piece)
        {
            bool firstMoveCapture = FirstMoveCapture;
            if (!piece.IsNull && SelectedPiece.IsNull && piece.Color == CurrentPlayer)
            {
                SelectedPiece = piece;
            }
            else if (!SelectedPiece.IsNull)
            {
                if (piece.IsNull)
                {
                    if (MakeMove(SelectedPiece, piece, FirstMoveMade, ref firstMoveCapture))
                    {
                        if (!MultipleJumps)
                            CurrentPlayer = (CurrentPlayer == EColor.White) ? EColor.Black : EColor.White;
                        else
                            FirstMoveMade = true;
                    }
                    if (Board.WhitePiecesCount == 0 || Board.BlackPiecesCount == 0)
                    {
                        IsGameOver = true;
                        // record statistics
                    }
                    FirstMoveCapture = firstMoveCapture;

                }
                SelectedPiece = new Piece(-1, -1);
            }
        }
        private bool MakeMove(Piece pieceToMove, Piece destination, bool firstMoveMade, ref bool firstMoveCapture)
        {
            if(NotStarted) NotStarted = false;
            Collection<int> possiblePositions = pieceToMove.Movement.GetPosibleMovements(Board.Pieces, new Position(pieceToMove.Line, pieceToMove.Column));

            int destinationIndex = destination.Line * 8 + destination.Column;

            if (!possiblePositions.Contains(destinationIndex))
                return false;
            return Board.MakeMove(pieceToMove, destination, firstMoveMade, ref firstMoveCapture);
        }

        public void ChangeTurn()
        {
            if (FirstMoveMade)
            {
                CurrentPlayer = (CurrentPlayer == EColor.White) ? EColor.Black : EColor.White;
                FirstMoveMade = false;
                FirstMoveCapture = false;
            }
        }

        public void Restart()
        {
            Board.Initialize();
            MultipleJumps = false;
            CurrentPlayer = EColor.Black;
            SelectedPiece = new Piece(-1, -1);
            IsGameOver = false;
            FirstMoveMade = false;
            FirstMoveCapture = false;
        }

        public void SetPieceMovement()
        {
            Board.SetPieceMovement();
        }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            info.AddValue(nameof(CurrentPlayer), CurrentPlayer);
            info.AddValue(nameof(FirstMoveMade), FirstMoveMade);
            info.AddValue(nameof(SelectedPiece), SelectedPiece);
            info.AddValue(nameof(MultipleJumps), MultipleJumps);
            info.AddValue(nameof(IsGameOver), IsGameOver);
            info.AddValue(nameof(Board), Board);
        }
        public void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
