using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dictionary
{
    class GuessGame
    {
        public GuessGame(WordCollection wordCollection)
        {
            WordCollection = wordCollection;
            UsedWords = new List<Word>();
            Random = new Random();
            Round = 0;
        }
        public WordCollection WordCollection { get; set; }
        public List<Word> UsedWords { get; set; }
        public Word CurrentWord { get; set; }
        public Random Random {  get; set; }

        public int Score {  get; set; }
        public int Round {  get; set; }

        public Word GetWord()
        {
            int index;
            do
            {
                index = Random.Next(0, WordCollection.Words.Count);

                CurrentWord = WordCollection.Words[index];

            } while (UsedWords.Contains(CurrentWord));

            UsedWords.Add(CurrentWord);
            Round++;

            return CurrentWord;
        }

        public bool VerifyGuess(string word)
        {
            if(CurrentWord.Equals(word))
            {
                Score++;
                return true;
            }
            return false;
        }

    }
}
