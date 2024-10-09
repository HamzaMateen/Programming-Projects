use std::{
    fs::File,
    io::{self, BufRead},
};

use clap::parser::ValueSource;
use prettytable::{cell, row, Cell, Row, Table};

mod cli;

fn main() -> std::io::Result<()> {
    let matches = cli::get_cli().get_matches();

    let lines = match matches.value_source("line").unwrap() {
        ValueSource::CommandLine => true,
        _ => false,
    };

    let words = match matches.value_source("word").unwrap() {
        ValueSource::CommandLine => true,
        _ => false,
    };

    let chars = match matches.value_source("char").unwrap() {
        ValueSource::CommandLine => true,
        _ => false,
    };

    let bytes = match matches.value_source("byte").unwrap() {
        ValueSource::CommandLine => true,
        _ => false,
    };
    // gotta read a file now
    let file_path = matches.get_one::<String>("path").unwrap();

    let file = File::open(file_path.as_str())?;
    let reader = io::BufReader::new(file);

    let mut cc = 0;
    let mut lc = 0;
    let mut wc = 0;
    let mut bc = 0;

    for line in reader.lines() {
        let mut line = line?;
        line = line.trim().to_string();

        wc += line.split_whitespace().collect::<Vec<&str>>().len();

        lc += 1;
        cc += line.chars().count();
        bc += line.len();
    }

    // summarize with a beautiful table
    let mut table = Table::new();
    let mut cells: Vec<Cell> = vec![];

    if chars {
        cells.push(Cell::new(format!("{}", cc).as_str()));
    }
    if lines {
        cells.push(Cell::new(format!("{}", lc).as_str()));
    }
    if words {
        cells.push(Cell::new(format!("{}", wc).as_str()));
    }
    if bytes {
        cells.push(Cell::new(format!("{}", bc).as_str()));
    }

    table.add_row(Row::new(cells));
    table.printstd();

    Ok(())
}
