// Library (written for the C# kit's judge): modern Java a translation into C# must keep -- generics,
// lambdas over java.util.function, ArrayList and TreeMap, StringBuilder, an interface and an abstract
// class with overrides, toString/equals/hashCode, a static nested class, a simple enum, exceptions,
// try-with-resources, switch on strings with fall-through, labelled loops, Java's text for doubles,
// floats, booleans and null, integer wrapping, java.util.Random with a seed.
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.TreeMap;
import java.util.function.BiFunction;
import java.util.function.Function;
import java.util.function.Predicate;

public class Library {
	enum Genre { NOVEL, POETRY, SCIENCE }

	interface Priced {
		int price();
	}

	static abstract class Item implements Priced, Comparable<Item> {
		protected final String title;
		private static int created;

		Item(String title) {
			this.title = title;
			created++;
		}

		abstract String kind();

		public int compareTo(Item o) {
			return title.compareTo(o.title);
		}

		public String toString() {
			return kind() + ":" + title;
		}
	}

	static class Book extends Item {
		final Genre genre;
		final int pages;

		Book(String title, Genre genre, int pages) {
			super(title);
			this.genre = genre;
			this.pages = pages;
		}

		String kind() {
			return "book";
		}

		public int price() {
			return 500 + pages * 3;
		}

		public boolean equals(Object o) {
			return o instanceof Book && ((Book)o).title.equals(title) && ((Book)o).pages == pages;
		}

		public int hashCode() {
			return title.hashCode() * 31 + pages;
		}
	}

	static class Magazine extends Item {
		final int issue;

		Magazine(String title, int issue) {
			super(title);
			this.issue = issue;
		}

		String kind() {
			return "magazine";
		}

		public int price() {
			return 250;
		}

		public String toString() {
			return super.toString() + "#" + issue;
		}
	}

	static class Ledger implements AutoCloseable {
		private final StringBuilder log = new StringBuilder();

		void add(String s) {
			log.append(s).append(';');
		}

		public void close() {
			System.out.println("ledger closed: " + log);
		}
	}

	static <T> List<T> filter(List<T> xs, Predicate<T> keep) {
		List<T> out = new ArrayList<T>();
		for (T x : xs) {
			if (keep.test(x)) {
				out.add(x);
			}
		}
		return out;
	}

	static <T, R> List<R> map(List<T> xs, Function<T, R> f) {
		List<R> out = new ArrayList<R>();
		for (T x : xs) {
			out.add(f.apply(x));
		}
		return out;
	}

	static int category(String word) {
		int score = 0;
		switch (word) {
			case "alpha":
				score += 1;
			case "beta":
				score += 10;
				break;
			case "gamma":
				score = -1;
				break;
			default:
				score = 100;
		}
		return score;
	}

	static int findPair(int[][] grid, int target) {
		int found = -1;
		outer:
		for (int r = 0; r < grid.length; r++) {
			for (int c = 0; c < grid[r].length; c++) {
				if (grid[r][c] < 0) {
					continue outer;
				}
				if (grid[r][c] == target) {
					found = r * 10 + c;
					break outer;
				}
			}
		}
		return found;
	}

	static int parse(String s) {
		try {
			return Integer.parseInt(s);
		} catch (NumberFormatException e) {
			return -1;
		} finally {
			System.out.println("parsed " + s);
		}
	}

	public static void main(String[] args) {
		List<Item> items = new ArrayList<Item>();
		items.add(new Book("Dune", Genre.NOVEL, 412));
		items.add(new Magazine("Wired", 7));
		items.add(new Book("Cosmos", Genre.SCIENCE, 365));
		items.add(new Book("Odes", Genre.POETRY, 88));
		for (Item i : items) {
			System.out.println(i + " costs " + i.price());
		}
		List<Item> cheap = filter(items, x -> x.price() < 1500);
		List<String> names = map(cheap, x -> x.title.toUpperCase());
		System.out.println("cheap: " + names + " of " + Item.created);

		Map<String, Integer> totals = new TreeMap<String, Integer>();
		for (Item i : items) {
			String k = i.kind();
			totals.put(k, totals.containsKey(k) ? totals.get(k) + i.price() : i.price());
		}
		for (Map.Entry<String, Integer> e : totals.entrySet()) {
			System.out.println(e.getKey() + " => " + e.getValue());
		}

		Book a = new Book("Dune", Genre.NOVEL, 412);
		System.out.println("equal " + a.equals(items.get(0)) + " same hash " + (a.hashCode() == items.get(0).hashCode())
				+ " hash " + a.hashCode() + " cmp " + a.compareTo(items.get(2)) + " genre " + a.genre + " " + a.genre.ordinal());

		BiFunction<Integer, Integer, Integer> add = (x, y) -> x + y;
		System.out.println("add " + add.apply(40, 2));

		System.out.println("categories " + category("alpha") + " " + category("beta") + " " + category("gamma") + " " + category("x"));
		int[][] grid = {{1, 2, 3}, {-1, 9, 9}, {4, 9, 6}};
		System.out.println("pair " + findPair(grid, 9) + " " + findPair(grid, 7));
		System.out.println("parse " + parse("123") + " " + parse("12x"));

		try (Ledger l = new Ledger()) {
			l.add("open");
			l.add("close");
		}

		double half = 0.5, big = 1.0e10, tiny = 0.00012, whole = 3.0;
		float f = 1.1f;
		boolean flag = true;
		String none = null;
		System.out.println(half + " " + big + " " + tiny + " " + whole + " " + f + " " + flag + " " + none + " " + (1.0 / 3.0));
		System.out.println("wrap " + (Integer.MAX_VALUE + 1) + " " + (long)Integer.MAX_VALUE * 2 + " " + (-7 / 2) + " " + (-7 % 2) + " " + (char)('a' + 2));
		System.out.println("round " + Math.round(2.5) + " " + Math.round(-2.5) + " " + Math.round(3.49f) + " " + Math.abs(Integer.MIN_VALUE));

		Random rng = new Random(42L);
		StringBuilder sb = new StringBuilder();
		for (int i = 0; i < 5; i++) {
			sb.append(rng.nextInt(100)).append(i < 4 ? "," : "");
		}
		System.out.println("random " + sb + " " + rng.nextDouble() + " " + rng.nextLong() + " " + rng.nextBoolean() + " " + rng.nextFloat());
		System.out.println("text " + "Hello".substring(1, 4) + " " + "a,b,,c,,".split(",").length + " " + "  pad ".trim() + "|" + "abc".indexOf('c') + " " + "Hello".hashCode());
	}
}
