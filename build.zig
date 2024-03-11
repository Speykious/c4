const std = @import("std");
const zcc = @import("compile_commands");

pub fn build(b: *std.build.Builder) void {
    var targets = std.ArrayList(*std.Build.CompileStep).init(b.allocator);

    const target = b.standardTargetOptions(.{});
    const mode = b.standardOptimizeOption(.{});

    const generalCompilerOptions = .{
        "-std=c11",
        "-pedantic",
        "-Wall",
        "-Werror",
        "-W",
        "-Wno-missing-field-initializers",
    };

    const c4_core = b.addStaticLibrary(.{
        .name = "c4-core",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_core.force_pic = true;
    c4_core.addCSourceFiles(&.{
        "src/core/alloc/arena.c",
        "src/core/math.c",
        "src/core/os.c",
    }, &generalCompilerOptions);
    targets.append(c4_core) catch @panic("OOM");

    const c4_app = b.addStaticLibrary(.{
        .name = "c4-app",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_app.force_pic = true;
    c4_app.addCSourceFiles(&.{
        "src/app/x11.c",
    }, &generalCompilerOptions);
    targets.append(c4_app) catch @panic("OOM");

    const c4_audio = b.addStaticLibrary(.{
        .name = "cingle-audio",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4_audio.force_pic = true;
    c4_audio.linkSystemLibrary("asound");
    c4_audio.addCSourceFiles(&.{
        "src/audio/audio.c",
    }, &generalCompilerOptions);
    targets.append(c4_audio) catch @panic("OOM");

    const c4 = b.addExecutable(.{
        .name = "c4",
        .target = target,
        .optimize = mode,
        .link_libc = true,
    });
    c4.linkLibrary(c4_core);
    c4.linkLibrary(c4_audio);
    c4.addCSourceFiles(&.{
        "src/main.c",
    }, &generalCompilerOptions);
    targets.append(c4) catch @panic("OOM");

    const run_step = b.addRunArtifact(c4);
    b.step("run", "Run for your fucking life.").dependOn(&run_step.step);
    b.installArtifact(c4);

    zcc.createStep(b, "cdb", targets.toOwnedSlice() catch @panic("OOM"));
}
