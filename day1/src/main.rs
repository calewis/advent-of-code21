use std::fs::File;
use std::io::{self, prelude::*, BufReader};

fn main() -> io::Result<()> {
    let file = File::open("./input.txt")?;
    let reader = BufReader::new(file);

    let mut data = Vec::new();
    for line in reader.lines() {
        let value = line.unwrap().parse::<i32>().unwrap();
        data.push(value);
    }

    let sum = &data
        .windows(2)
        .map(|w| if w[1] > w[0] { 1 } else { 0 })
        .sum::<i32>();
    println!("{}", sum);

    Ok(())
}
