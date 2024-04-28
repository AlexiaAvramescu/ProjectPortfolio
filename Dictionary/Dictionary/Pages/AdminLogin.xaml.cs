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
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace Dictionary.Pages
{
    /// <summary>
    /// Interaction logic for AdminLogin.xaml
    /// </summary>
    public partial class AdminLogin : Page
    {
        public AdminLogin(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
        }

        private void loginBtn_Click(object sender, RoutedEventArgs e)
        {
            if ((DataContext as WordCollection).Verify(UsernameTextBox.Text, PasswordBox.Password))
                ((MainWindow)Application.Current.MainWindow).MainFrame.Navigate(new AdminView(this.DataContext));
            else MessageBox.Show("Incorrect.");
        }

        private void button_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.GoBack();
        }
    }
}
