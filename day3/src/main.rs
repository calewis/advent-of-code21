fn main() {
    let input: Vec<u16> = include_str!("input.txt")
        .lines()
        .map(|l| u16::from_str_radix(l, 2).unwrap())
        .collect();

    let marjority_threshold = input.len() / 2;

    let mut count: [usize; 12] = [0; 12];
    for num in input.iter() {
        for i in 0..12 {
            count[i] += if num & (1 << i) > 0 { 1 } else { 0 };
        }
    }

    let signal = count.map(|i| i / marjority_threshold);
    let mut gamma: usize = 0;
    for i in 0..12 {
        gamma += signal[i] << i;
    }
    let epsilon = gamma ^ 0xFFF;
    println!("Part 1 Power: {}", epsilon * gamma);

    //
    // Part 2
    //

    // Oxygen First
    let mut oxygen: Vec<u16> = input.clone();
    let mut o2reading: usize = 0;
    for i in 0..12 {
        let (ones, zeros): (Vec<u16>, Vec<u16>) =
            oxygen.iter().partition(|n| (*n & (1 << (11 - i))) > 0);
        if ones.len() >= zeros.len() {
            oxygen = ones;
        } else {
            oxygen = zeros;
        }
        if oxygen.len() == 1 {
            o2reading = oxygen[0] as usize;
            break;
        }
    }

    // CO2 Second
    let mut co2: Vec<u16> = input.clone();
    let mut co2_reading: usize = 0;
    for i in 0..12 {
        let (ones, zeros): (Vec<u16>, Vec<u16>) =
            co2.iter().partition(|n| (*n & (1 << (11 - i))) > 0);
        if zeros.len() =< ones.len() {
            co2 = zeros;
        } else {
            co2 = ones;
        }
        if co2.len() == 1 {
            co2_reading = co2[0] as usize;
            break;
        }
    }

    println!("o2: {}, cO2: {}", o2reading, co2_reading);
    println!("Part 2: {}", o2reading * co2_reading);
}
