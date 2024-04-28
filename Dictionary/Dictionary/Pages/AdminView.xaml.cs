using Microsoft.Win32;
using System;
using System.Collections.Generic;
using System.IO;
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
    /// Interaction logic for AdminView.xaml
    /// </summary>
    public partial class AdminView : Page
    {
        public AdminView(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
        }

        private String ImageSelected {  get; set; }

        private void SelectImgBtn_Click(object sender, RoutedEventArgs e)
        {
            string projectFolderPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory);

            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = projectFolderPath;

            openFileDialog.DefaultExt = ".jpg";
            openFileDialog.Filter = "Image Files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif";

            Nullable<bool> result = openFileDialog.ShowDialog();

            if (result == true)
            {
                ImageSelected = openFileDialog.FileName;

                try
                {
                    string resourcesDirectory = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources");

                    if (!Directory.Exists(resourcesDirectory))
                    {
                        Directory.CreateDirectory(resourcesDirectory);
                    }

                    string fileName = System.IO.Path.GetFileName(ImageSelected);

                    string destinationFilePath = System.IO.Path.Combine(resourcesDirectory, fileName);

                    File.Copy(ImageSelected, destinationFilePath, true);

                    string relativePath = System.IO.Path.Combine("Resources", fileName);
                    imagePath.Text = relativePath;
                }
                catch (Exception ex)
                {
                    MessageBox.Show("Eroare la selectarea imaginii: " + ex.Message);
                }

            }
        }

        private void AddWordBtn_Click(object sender, RoutedEventArgs e)
        {
            if (!(DataContext as WordCollection).AddWord(value.Text, description.Text, category.Text, ImageSelected))
            { MessageBox.Show("Word, desctription, or category field empty."); }
            value.Clear();
            description.Clear();
        }

        private void BackArrowBtn_Click(object sender, RoutedEventArgs e)
        {
            FileManager.saveToFile(DataContext as WordCollection);
            ((MainWindow)Application.Current.MainWindow).MainFrame.GoBack();
        }

        private void ListView_SelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            (DataContext as WordCollection).SelectedWord = (sender as ListBox).SelectedItem as Word;
        }

        private void ListView_Selected(object sender, RoutedEventArgs e)
        {
            UpdateWordWindow updateWindow = new UpdateWordWindow(DataContext);
            updateWindow.ShowDialog();
        }

        private void searchBar_TextChanged(object sender, TextChangedEventArgs e)
        {
            (DataContext as WordCollection).SearchFor(searchBar.Text, "");
        }
    }
}
