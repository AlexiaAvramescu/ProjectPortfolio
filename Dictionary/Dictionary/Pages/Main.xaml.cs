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
    /// Interaction logic for Main.xaml
    /// </summary>
    public partial class Main : Page
    {
        public Main(object dContext)
        {
            InitializeComponent();
            DataContext = dContext;
            AdminLoginPage = new AdminLogin(this.DataContext);
            AdminViewPage = new AdminView(this.DataContext);
            SearchPage = new Search(this.DataContext);
            EntertainmentPage = new Entertainment(this.DataContext);
        }

        private AdminLogin AdminLoginPage { get; set; }
        private AdminView AdminViewPage { get; set; }
        private Search SearchPage { get; set; }
        private Entertainment EntertainmentPage { get; set; }

        private void btnAdmin_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.Navigate(AdminLoginPage);
        }

        private void btnSearch_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.Navigate(SearchPage);
        }

        private void btnEntertain_Click(object sender, RoutedEventArgs e)
        {
            ((MainWindow)Application.Current.MainWindow).MainFrame.Navigate(EntertainmentPage);
        }
    }
}
