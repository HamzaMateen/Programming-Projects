use std::cmp::min_by_key;
use std::fs::File;
use std::io::{self, BufRead};
use std::path::Path;

mod cli;

fn main() -> io::Result<()> {
    let cut_cli = cli::get_cli();

    let matches = cut_cli.get_matches();

    let fields = matches
        .get_many::<u32>("field")
        .expect("cut: incorrect usage. Try 'cut --help' for more information.")
        .collect::<Vec<&u32>>();

    let file_path = matches
        .get_one::<String>("path")
        .expect("Path not configured correctly.");

    let delimeter = matches.get_one::<String>("delimeter").unwrap();

    // stdin reading support
    if *file_path == "-".to_string() {
        let mut line = String::new();

        loop {
            io::stdin()
                .read_line(&mut line)
                .expect("can't read line from std input");

            line = line[0..line.len() - 1].to_string();
            print_fields(&line, delimeter, &fields);
            line.clear();
        }
    }

    // file reading support
    if let Ok(lines) = read_lines(file_path) {
        for line in lines.flatten() {
            print_fields(&line, delimeter, &fields)
        }
    }

    Ok(())
}

fn print_fields(line: &String, delimeter: &String, fields: &Vec<&u32>) {
    let words: Vec<&str> = line.split(delimeter).collect();
    let delim_print_count = min_by_key(fields.len(), words.len(), |x| *x);

    for (i, &&field_num) in (&(*fields)).iter().enumerate() {
        if field_num <= (words.len() as u32) {
            print!("{}", words[(field_num - 1) as usize]);
            if i < delim_print_count - 1 {
                print!("{}", delimeter);
            }
        } else {
            print!("\t");
        }
    }
    println!();
}

fn read_lines<P>(filename: P) -> io::Result<io::Lines<io::BufReader<File>>>
where
    P: AsRef<Path>,
{
    let file = File::open(filename)?;
    Ok(io::BufReader::new(file).lines())
}
