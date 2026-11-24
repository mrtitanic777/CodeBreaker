import java.util.*;

// Arrays.asList: a list of a fixed size over the array itself. A set or a sort is seen in the array, a change of size throws.
public class AsListKit {
    public static void main(String[] args) {
        String[] names = {"c", "a", "b"};
        List<String> view = Arrays.asList(names);
        view.set(0, "z");
        System.out.println(names[0] + " " + view);
        Collections.sort(view);
        System.out.println(names[0] + names[1] + names[2] + " " + view.size());
        names[1] = "q";
        System.out.println(view.get(1));
        try {
            view.add("d");
            System.out.println("added");
        } catch (UnsupportedOperationException e) {
            System.out.println("fixed size");
        }
        List<Integer> loose = Arrays.asList(1, 2, 3);
        loose.set(2, 9);
        try {
            loose.remove(0);
            System.out.println("removed");
        } catch (UnsupportedOperationException e) {
            System.out.println("fixed " + loose);
        }
    }
}
