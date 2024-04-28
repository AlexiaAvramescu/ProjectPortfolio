using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Remoting.Contexts;
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
    /// Interaction logic for Search.xaml
    /// </summary>
    public partial class Search : Page
    {
        public Search(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.GoBack();
        }

        private void searchBar_TextChanged(object sender, TextChangedEventArgs e)
        {
            (DataContext as WordCollection).SearchFor(searchBar.Text, category.Text);
        }

        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            var selectedWord = (DataContext as WordCollection).SelectedWord;
            selectedWord = (sender as ListBox).SelectedItem as Word;

            if (selectedWord == null) return;

            value.Text = selectedWord.Value;
            description.Text = selectedWord.Description;
            wordCategory.Text = selectedWord.Category;
            image.Source = new BitmapImage(new Uri(selectedWord.ImagePath));
        }
    }
}
