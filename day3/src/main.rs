fn main() {
    let input: Vec<u16> = include_str!("input.txt")
        .lines()
        .map(|l| u16::from_str_radix(l, 2).unwrap())
        .collect();

    let marjority_threshold = input.len() / 2;

    let mut count: [usize; 12] = [0; 12];
    for num in input {
        for i in 0..12 {
            count[i] += if num & (1<<i) > 0 {1} else {0}; 
        }
    }

    let signal = count.map(|i| i / marjority_threshold);
    let mut gamma: usize = 0;
    for i in 0..12 {
        gamma += signal[i] << i;
    }
    let epsilon = gamma ^ 0xFFF;
    println!("Power: {}", epsilon * gamma);

}
