import { execFile } from "node:child_process";
import { appendFileSync } from "node:fs";
import { promisify } from "node:util";
import { readVersion } from "./version.mjs";

const exec = promisify(execFile);

function tagValue(tag) {
	const m = tag.match(/^v(\d+)\.(\d{1,2})$/);
	return m ? Number(m[1]) * 100 + Number(m[2]) : 0;
}

async function latestTag() {
	const { stdout } = await exec("git", [
		"ls-remote",
		"--tags",
		"origin",
		"refs/tags/v*",
	]);
	const tags = stdout
		.split("\n")
		.map((line) =>
			line
				.split("\t")[1]
				?.replace(/^refs\/tags\//, "")
				.replace(/\^\{\}$/, ""),
		)
		.filter((t) => t && tagValue(t) !== null)
		.sort((a, b) => tagValue(a) - tagValue(b));
	return tags.at(-1) ?? null;
}

try {
	const { num, version, tag } = await readVersion();
	const latest = await latestTag();
	const needed = !latest || num > tagValue(latest);
	const reason = !latest
		? "no existing tags"
		: needed
			? `bumped ${latest} -> ${tag}`
			: `${tag} <= ${latest}`;
	console.log(`release needed: ${needed} (${reason})`);
	if (process.env.GITHUB_OUTPUT) {
		appendFileSync(
			process.env.GITHUB_OUTPUT,
			`version=${version}\ntag=${tag}\nneeded=${needed}\n`,
		);
	}
} catch (err) {
	console.error(`::error::${err.message}`);
	process.exit(1);
}
