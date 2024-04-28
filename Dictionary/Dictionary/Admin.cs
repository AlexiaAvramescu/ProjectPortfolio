using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Dictionary
{
    class Admin
    {
        public Admin(string name, string passward)
        {
            Name = name;
            Passward = passward;
        }
        public string Name { get; set; }
        public string Passward { get; set; }
    }
}
