using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.Models
{
    public struct Position
    {
        public int Line { get; set; }
        public int Column { get; set; }
        public Position(int line, int column) { Line = line; Column = column; }
    }
}
