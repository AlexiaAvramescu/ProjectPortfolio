using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dictionary
{
    class WordCollection
    {
        public WordCollection()
        {
            Words = new ObservableCollection<Word>();
            Categories = new ObservableCollection<String>();
            SearchedWords = new ObservableCollection<Word>();
            _defaultImg = System.IO.Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "Resources\\default-image.jpg");
        }

        public ObservableCollection<Word> Words { get; set; }
        public ObservableCollection<String> Categories
        {
            get;
            set;
        }
        public ObservableCollection<Word> SearchedWords { get; set; }
        public Word SelectedWord
        {
            get;
            set;
        }

        public List<Admin> Admins { get; set; }

        private string _defaultImg;


        private bool ValidateInput(string word)
        {
            return string.IsNullOrEmpty(word) ? false : true;
        }
        public bool AddWord(string word, string description, string category, string imgPath)
        {
            if (!ValidateInput(imgPath))
                imgPath = _defaultImg;
            if (ValidateInput(word) && ValidateInput(description) && ValidateInput(category) && Words.FirstOrDefault(item => item.Equals(word)) == null)
            {
                if (Categories.FirstOrDefault(item => item.Equals(category)) == null)
                {
                    AddCategory(category);
                }
                Word newWord = new Word() { Value = word, Category = category, Description = description, ImagePath = imgPath };
                Words.Add(newWord);
                return true;
            }
            return false;
        }

        public void AddCategory(string category)
        {
            if (ValidateInput(category))
            {
                Categories.Add(category);
            }
        }

        public bool RemoveWord()
        {
            return Words.Remove(SelectedWord);
        }

        public void SearchFor(string word, string category)
        {
            if (!ValidateInput(word)) return;
            SearchedWords.Clear();
            var foundWords = Words.Where(item => item.Value.StartsWith(word, StringComparison.OrdinalIgnoreCase));

            if(!string.IsNullOrEmpty(category)) 
            {
                foundWords = foundWords.Where(item=> item.Category.Equals(category));
            }

            foreach (var item in foundWords)
            {
               SearchedWords.Add(item);
            }
        }

        public bool Verify(string username, string password)
        {
            if (username == "asd" && password == "asd")
                return true;
            return false;
        }
    }
}
