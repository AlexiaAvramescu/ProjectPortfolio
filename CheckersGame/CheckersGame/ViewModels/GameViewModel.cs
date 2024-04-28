using CheckersGame.Commands;
using CheckersGame.Models;
using CheckersGame.Services;
using CheckersGame.Views;
using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.ComponentModel;
using System.IO;
using System.Linq;
using System.Runtime.Serialization;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Input;
using System.Windows.Media.Effects;

namespace CheckersGame.ViewModels
{
    internal class GameViewModel : BaseViewModel
    {
        public GameViewModel()
        {
            FileService = new FileManager("game.json", "preferences.json");

            bool multipleJumps = FileService.LoadPreferences<bool>();

            Statistics = FileService.LoadData<Statistics>("statistics.json");
            if (Statistics == null) { Statistics = new Statistics(true); }

            Game = new Game(multipleJumps);

            SquareClickCommand = new RelayCommand(SquareClick);
            LoadGameCommand = new RelayCommand(LoadGame);
            SaveGameCommand = new RelayCommand(SaveGame);
            ShowAboutCommand = new RelayCommand(ShowAboout);
            ShowStatisticsCommand = new RelayCommand(ShowStatistics);
            ChangeTurnCommand = new RelayCommand(ChangeTurn);
            StartNewGameCommand = new RelayCommand(StartNewGame);
        }

        private Game _game;
        private FileManager _fileService;
        private Statistics _statistics;
        public Game Game
        {
            get { return _game; }
            set { _game = value; OnPropertyChanged(nameof(Game)); }
        }
        public FileManager FileService { get; set; }

        public Statistics Statistics { get; set; }
        public ICommand SquareClickCommand { get; set; }
        public ICommand LoadGameCommand { get; set; }
        public ICommand SaveGameCommand { get; set; }
        public ICommand ShowStatisticsCommand { get; set; }
        public ICommand ShowAboutCommand { get; set; }
        public ICommand ChangeTurnCommand { get; set; }
        public ICommand StartNewGameCommand { get; set; }


        private void SquareClick(object parameter)
        {
            var piece = parameter as Piece;
            if (piece != null)
                Game.OnPieceClicked(piece);

            if (Game.IsGameOver)
            {
                Statistics.GamesPlayed += 1;
                if (Game.Board.BlackPiecesCount == 0)
                    Statistics.WhiteWins += 1;
                else
                    Statistics.BlackWins += 1;
                FileService.SaveData<Statistics>(Statistics, "statistics.json");
            }
        }

        public void ChangeTurn(object parameter)
        {
            Game.ChangeTurn();
        }

        private void LoadGame(object parameter)
        {
            Game newGame = FileService.LoadData<Game>("game.json");
            if (newGame == null)
            {
                MessageBox.Show("Load failed.");
                return;
            }
            Game = newGame;
            Game.SetPieceMovement();
        }
        private void SaveGame(object parameter)
        {
            if (FileService.SaveData<Game>(Game, "game.jsom"))
                MessageBox.Show("Game Saved.");
            else
                MessageBox.Show("Game failed to save.");
        }

        private void ShowAboout(object parameter)
        {
            AboutView aboutPage = new AboutView();
            aboutPage.Show();
        }
        private void ShowStatistics(object parameter)
        {
            StatisticsViewModel statisticsVM = new StatisticsViewModel(Statistics);
            StatisticsView statisticsView = new StatisticsView(statisticsVM);
            statisticsView.Show();
        }
        private void StartNewGame(object parameter)
        {
            Game.Restart();
        }
    }
}
