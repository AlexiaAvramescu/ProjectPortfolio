using System.Collections.ObjectModel;

namespace CheckersGame.Models
{
    public interface IMovement
    {
        Collection<int> GetPosibleMovements(ObservableCollection<Piece> board, Position fromPos);
    }
}