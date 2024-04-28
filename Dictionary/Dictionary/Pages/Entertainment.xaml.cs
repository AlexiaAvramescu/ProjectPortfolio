using System;
using System.Collections.Generic;
using System.IO.Packaging;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Dictionary.Pages
{
    /// <summary>
    /// Interaction logic for Entertainment.xaml
    /// </summary>
    public partial class Entertainment : Page
    {
        public Entertainment(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
            Game = new GuessGame(DataContext as WordCollection);
            DisplayHint();
        }

        GuessGame Game { get; set; }

        private void goBackBtn_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.GoBack();
        }

        private void nextBtn_Click(object sender, RoutedEventArgs e)
        {
            if (nextBtn.Content == "Finish")
            {
                Game.VerifyGuess(guess.Text);
                ((MainWindow)Application.Current.MainWindow).MainFrame.GoBack();
                MessageBox.Show($"You have obtained {Game.Score} out of 5.");
                return;
            }
            if (!Game.VerifyGuess(guess.Text))
                MessageBox.Show($"The correct word was: {Game.CurrentWord.ToString()}");
            if (Game.Round == 4)
                nextBtn.Content = "Finish";

            guess.Clear();
            DisplayHint();
        }

        private void DisplayHint()
        {
            Word word = Game.GetWord();
            if (DisplayImage(word))
            {
                image.Visibility = Visibility.Visible;
                image.Source = new BitmapImage(new Uri(word.ImagePath));
                description.Text = "";
            }
            else
            {
                image.Visibility = Visibility.Collapsed;
                description.Text = word.Description;
            }
        }

        private bool DisplayImage(Word word)
        {
            if (word.ImagePath.Contains("default"))
                return false;
            return Game.Random.Next(0, 10) % 2 == 0 ? true : false;
        }
    }
}
