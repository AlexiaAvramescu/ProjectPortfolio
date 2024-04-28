using Microsoft.Win32;
using System;
using System.Collections.Generic;
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
using System.Windows.Shapes;

namespace Dictionary.Pages
{
    /// <summary>
    /// Interaction logic for UpdateWordWindow.xaml
    /// </summary>
    public partial class UpdateWordWindow : Window
    {
        public UpdateWordWindow(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
        }

        private void selectImgBtn_Click(object sender, RoutedEventArgs e)
        {
            string imgPath = (DataContext as WordCollection).SelectedWord.ImagePath;
            string projectFolderPath = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory);

            OpenFileDialog openFileDialog = new OpenFileDialog();

            openFileDialog.InitialDirectory = projectFolderPath;

            openFileDialog.DefaultExt = ".jpg";
            openFileDialog.Filter = "Image Files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif";

            Nullable<bool> result = openFileDialog.ShowDialog();

            if (result == true)
            {
                imgPath = openFileDialog.FileName;
            }
        }

        private void removeWordBtn_Click(object sender, RoutedEventArgs e)
        {
            if ((DataContext as WordCollection).RemoveWord())
                this.Close();
            MessageBox.Show("Word could not be removed.");
        }
    }
}
