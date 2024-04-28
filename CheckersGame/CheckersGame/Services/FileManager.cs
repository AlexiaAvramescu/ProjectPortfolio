using Newtonsoft.Json;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;


namespace CheckersGame.Services
{
    internal class FileManager
    {
        private readonly string _filePathDefault;
        private readonly string _filePathPreferences;

        public FileManager(string filePath, string filePathPreferances)
        {
            _filePathDefault = filePath;
            _filePathPreferences = filePathPreferances;
        }

        public bool SaveData<T>(T data, string filePath)
        {
            try
            {
                string jsonData = JsonConvert.SerializeObject(data);
                File.WriteAllText(filePath, jsonData);
                return true;
            }
            catch (Exception ex)
            {
                return false;
            }
        }

        public T LoadData<T>(string filePath)
        {
            try
            {
                if (File.Exists(filePath))
                {
                    string jsonData = File.ReadAllText(filePath);
                    return JsonConvert.DeserializeObject<T>(jsonData);
                }
                else
                {
                    Console.WriteLine("File does not exist.");
                    return default;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error loading data: {ex.Message}");
                return default;
            }
        }

        public T LoadPreferences<T>()
        {
            try
            {
                if (File.Exists(_filePathPreferences))
                {
                    string jsonData = File.ReadAllText(_filePathDefault);
                    return JsonConvert.DeserializeObject<T>(jsonData);
                }
                else
                {
                    Console.WriteLine("File does not exist.");
                    return default;
                }
            }
            catch (Exception ex)
            {
                Console.WriteLine($"Error loading data: {ex.Message}");
                return default;
            }
        }
    }
}
