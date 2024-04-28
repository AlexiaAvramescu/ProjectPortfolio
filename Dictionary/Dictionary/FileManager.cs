using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Newtonsoft.Json;
using System.Collections.ObjectModel;
using System.IO;

namespace Dictionary
{
    class FileManager
    {
        public static void saveToFile(WordCollection wordCollection)
        {
            string json = JsonConvert.SerializeObject(wordCollection.Categories);
            File.WriteAllText("categoriesData.json", json);

            json = JsonConvert.SerializeObject(wordCollection.Words);
            File.WriteAllText("wordsData.json", json);
        }

        public static WordCollection loadFromFile() 
        {
            string json = File.ReadAllText("categoriesData.json");
            List<String> categoriesList = JsonConvert.DeserializeObject<List<String>>(json);
            ObservableCollection<String> categories = new ObservableCollection<String>(categoriesList);

            json = File.ReadAllText("wordsData.json");
            List<Word> wordsList = JsonConvert.DeserializeObject<List<Word>>(json);
            ObservableCollection<Word> words = new ObservableCollection<Word>(wordsList);

            WordCollection wordCollection = new WordCollection();
            wordCollection.Words = words;
            wordCollection.Categories = categories;
            return wordCollection;
        }
    }
}
