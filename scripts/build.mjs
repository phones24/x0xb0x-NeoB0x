import { spawnSync } from "node:child_process";
import { copyFileSync, mkdirSync } from "node:fs";
import { readVersion } from "./version.mjs";

const FLASH_BUDGETS = { atmega162: 15872, atmega2561: 245760 };
const TARGETS = ["atmega2561", "atmega162"];

function fail(msg) {
	console.error(`::error::${msg}`);
	process.exit(1);
}

function run(cmd, args) {
	const res = spawnSync(cmd, args, { stdio: "inherit" });
	if (res.status !== 0)
		fail(`${cmd} ${args.join(" ")} failed (${res.status ?? res.signal})`);
}

function elf(mcu, version) {
	return `out/${mcu}/NeoB0x_${version}_${mcu}.elf`;
}

function checkFlash(mcu, version) {
	const res = spawnSync("avr-size", [elf(mcu, version)], { encoding: "utf8" });
	const cols =
		res.status === 0 ? res.stdout?.split("\n")[1]?.trim().split(/\s+/) : null;
	const size = cols ? Number(cols[0]) + Number(cols[1]) : NaN;

	if (!Number.isFinite(size)) {
		fail(`avr-size failed for ${elf(mcu, version)}`);
	}

	if (size > FLASH_BUDGETS[mcu])
		fail(
			`${elf(mcu, version)} exceeds flash budget (${size} > ${FLASH_BUDGETS[mcu]})`,
		);
}

function collectFiles(version, runNum) {
	mkdirSync("release", { recursive: true });

	for (const mcu of TARGETS) {
		copyFileSync(
			`out/${mcu}/NeoB0x_${version}_${mcu}.hex`,
			`release/NeoB0x_v${version}+build.${runNum}_${mcu}.hex`,
		);
	}
}

const [cmd, arg] = process.argv.slice(2);

try {
	const { version, tag } = await readVersion();

	if (cmd === "check-tag") {
		if (
			spawnSync("git", [
				"ls-remote",
				"--exit-code",
				"origin",
				`refs/tags/${tag}`,
			]).status === 0
		) {
			fail(
				`tag ${tag} already exists - bump the VERSION macro in src/main.h before pushing to main`,
			);
		}
	} else if (cmd === "build") {
		for (const mcu of TARGETS) {
			run("make", [`MCU=${mcu}`]);
		}

		for (const mcu of TARGETS) {
			checkFlash(mcu, version);
		}
	} else if (cmd === "collect") {
		if (!arg) {
			fail("usage: build.mjs collect <run-number>");
		}

		collectFiles(version, arg);
	} else {
		fail("usage: build.mjs check-tag | build | collect <run-number>");
	}
} catch (err) {
	fail(err.message);
}
