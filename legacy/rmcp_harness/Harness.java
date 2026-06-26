// Headless driver for RetroMCP-Java: runs a single task via MCP.performTask with a
// plain-text logger (no jansi), so output is capturable. Usage: Harness <task> [workspaceDir]
// Targets the v1.2.1+ API (MCP.options field; getWorkingDir/getOptions concrete; warning/error added).
import org.mcphackers.mcp.MCP;
import org.mcphackers.mcp.Options;
import org.mcphackers.mcp.tasks.Task;
import org.mcphackers.mcp.tasks.mode.TaskMode;
import org.mcphackers.mcp.tools.versions.json.Version;
import org.json.JSONObject;

import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

public class Harness extends MCP {
    private Version version;

    Harness(Path wd) throws Exception {
        this.options = new Options(this, wd.resolve("options.cfg"));
        this.options.workingDir = wd;          // drives getWorkingDir()
        this.options.side = Task.Side.ANY;
        String json = new String(Files.readAllBytes(wd.resolve("conf/version.json")));
        this.version = Version.from(new JSONObject(json));
    }

    public void setProgressBars(List<Task> t, TaskMode m) {}
    public void clearProgressBars() {}
    public void log(String s) { System.out.println(s); }
    public void warning(String s) { System.out.println("[WARN] " + s); }
    public void error(String s) { System.out.println("[ERROR] " + s); }
    public Version getCurrentVersion() { return version; }
    public void setCurrentVersion(Version v) { this.version = v; }
    public void setProgress(int i, String s) {}
    public void setProgress(int i, int p) {}
    public void setActive(boolean b) {}
    public boolean yesNoInput(String a, String b) { return true; }
    public String inputString(String a, String b) { return ""; }
    public void showMessage(String a, String b, int c) { System.out.println("[MSG] " + a + ": " + b); }
    public void showMessage(String a, String b, Throwable t) {
        System.out.println("[MSG] " + a + ": " + b);
        if (t != null) t.printStackTrace(System.out);
    }
    public boolean updateDialogue(String a, String b) { return false; }

    private static TaskMode mode(String name) {
        if ("decompile".equals(name)) return TaskMode.DECOMPILE;
        if ("recompile".equals(name)) return TaskMode.RECOMPILE;
        if ("reobfuscate".equals(name)) return TaskMode.REOBFUSCATE;
        if ("build".equals(name)) return TaskMode.BUILD;
        throw new IllegalArgumentException("unknown task: " + name);
    }

    public static void main(String[] args) throws Exception {
        String task = args.length > 0 ? args[0] : "recompile";
        Path wd = Paths.get(args.length > 1 ? args[1] : ".").toAbsolutePath().normalize();
        System.out.println(">>> RetroMCP headless: task=" + task + " side=CLIENT wd=" + wd);
        Harness h = new Harness(wd);
        long t0 = System.currentTimeMillis();
        boolean ok;
        try {
            ok = h.performTask(mode(task), Task.Side.CLIENT);
        } catch (Throwable e) {
            System.out.println(">>> EXCEPTION: " + e);
            e.printStackTrace(System.out);
            ok = false;
        }
        System.out.println(">>> result: " + (ok ? "SUCCESS" : "FAILED")
                + "  (" + (System.currentTimeMillis() - t0) / 1000.0 + "s)");
    }
}
