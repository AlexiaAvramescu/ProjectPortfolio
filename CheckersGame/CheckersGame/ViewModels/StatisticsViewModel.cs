using CheckersGame.Models;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace CheckersGame.ViewModels
{
    internal class StatisticsViewModel : BaseViewModel
    {
        public StatisticsViewModel(Statistics statistics) { 
            BlackWins = statistics.BlackWins;
            WhiteWins = statistics.WhiteWins;
            GamesPlayed = statistics.GamesPlayed;
        }

        private int BlackWins {  get; set; }
        public int WhiteWins { get; set; }
        public int GamesPlayed { get; set; }
    }
}
