using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dictionary
{
    class Word : INotifyPropertyChanged
    {
        public Word() { }

        private String _value;

        public String Value
        {
            get { return _value; }
            set 
            { 
                _value = value;
                NotifyPropertyChanged("Value");
            }
        }

        private String _description;

        public String Description
        {
            get { return _description; }
            set 
            { 
                _description = value; 
                NotifyPropertyChanged("Description");

            }
        }

        private String _imagePath;

        public String ImagePath
        {
            get { return _imagePath; }
            set 
            { 
                _imagePath = value; 
                NotifyPropertyChanged("ImagePath");
            }
        }

        private String _category;

        public String Category
        {
            get { return _category; }
            set 
            { 
                _category = value;
                NotifyPropertyChanged("Category");
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        public void NotifyPropertyChanged(string propertyName)
        {
            PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }

        public bool Equals(string value)
        {
            return value == this.Value;
        }

        public override string ToString()
        {
            return Value;
        }
    }
}
