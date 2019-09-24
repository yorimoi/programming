using System;

partial class Cal
{
    static int Main() {
        int i = 0;
        string[] argv = System.Environment.GetCommandLineArgs();
        if (argv.Length != 2) {
            Console.Error.WriteLine("Invalid arguments");
            return 1;
        }

        i = tokenize(argv[1]);

        return i;
    }

   unsafe  static int tokenize(string str) {
        int i = 0;
        int val = strtoi(ref str, &i);

        for (i += 1; i < str.Length; ++i) {
            if (str[i] == '+') {
                ++i;
                val += strtoi(ref str, &i);
                continue;
            }
            if (str[i] == '-') {
                ++i;
                val -= strtoi(ref str, &i);
                continue;
            }

            Console.Error.WriteLine("Non expected: \"" + str[i] + "\"");
            Environment.Exit(1);
        }

        return val;
    }

    unsafe static int strtoi(ref string s) {
        int i = 0;
        return strtoi(ref s, &i);
    }

    unsafe static int strtoi(ref string s, int *i) {
        int val = 0;

        while (*i < s.Length) {
            if (s[*i] < '0' || '9' < s[*i]) {
                --(*i);
                return val;
            }
            val = val * 10 + (s[*i] - '0');
            ++(*i);
        }

        return val;
    }
}
