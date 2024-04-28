using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.Models
{
    internal class Statistics : ISerializable
    {
        public Statistics()
        {}

        public Statistics(bool multipleJumps)
        {
            BlackWins = 0;
            WhiteWins = 0;
            GamesPlayed = 0;
        }

        private int _whiteWins;
        private int _blackWins;
        private int _gamesPlayed;

        public int WhiteWins { get { return _whiteWins; } set { _whiteWins = value; } }
        public int BlackWins { get { return _blackWins; } set { _blackWins = value; } }
        public int GamesPlayed { get { return _gamesPlayed; } set { _gamesPlayed = value; } }
        public bool MultipleJumps { get; set; }

        public void GetObjectData(SerializationInfo info, StreamingContext context)
        {
            throw new NotImplementedException();
        }
    }
}
