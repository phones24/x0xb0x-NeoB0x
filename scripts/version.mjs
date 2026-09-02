import { readFile } from "node:fs/promises";

export async function readVersion(path = "src/main.h") {
	const src = await readFile(path, "utf8");
	const m = src.match(/^#\s*define\s+VERSION\s+(\d+)\s*$/m);
	if (!m) {
		throw new Error(`cannot parse VERSION macro from ${path}`);
	}

	const num = Number(m[1]);
	const major = Math.floor(num / 100);
	const minor = String(num % 100).padStart(2, "0");

	return { num, version: `${major}.${minor}`, tag: `v${major}.${minor}` };
}
