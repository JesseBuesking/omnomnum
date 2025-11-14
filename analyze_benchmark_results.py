#!/usr/bin/env python3
import csv
import sys

# Read the CSV
data = []
with open('benchmark_history/means_with_deltas.csv', 'r') as f:
    reader = csv.DictReader(f)
    for row in reader:
        data.append(row)

# Calculate average percentage change per commit
commit_impacts = []
for i, row in enumerate(data):
    if i == 0:  # First commit has no deltas
        continue
    
    # Extract all _pct columns
    pct_changes = []
    for key in row.keys():
        if key.endswith('_pct') and row[key]:
            try:
                val = float(row[key])
                pct_changes.append(val)
            except:
                pass
    
    if pct_changes:
        avg_pct = sum(pct_changes) / len(pct_changes)
        abs_avg_pct = sum(abs(x) for x in pct_changes) / len(pct_changes)
        max_regression = min(pct_changes)
        max_improvement = max(pct_changes)
        
        # Extract commit info
        filename = row['file'].split('/')[-1]
        parts = filename.replace('.json', '').split('-', 1)
        commit_num = parts[0]
        commit_sha = parts[1][:12] if len(parts) > 1 else "unknown"
        
        commit_impacts.append({
            'num': commit_num,
            'sha': commit_sha,
            'avg_pct': avg_pct,
            'abs_avg_pct': abs_avg_pct,
            'max_regression': max_regression,
            'max_improvement': max_improvement,
        })

# Sort by absolute average impact
commit_impacts.sort(key=lambda x: abs(x['avg_pct']), reverse=True)

print("=" * 80)
print("TOP 20 COMMITS BY PERFORMANCE IMPACT")
print("=" * 80)
print()

for i, commit in enumerate(commit_impacts[:20], 1):
    impact_type = "REGRESSION" if commit['avg_pct'] > 0 else "IMPROVEMENT"
    print(f"{i}. Commit {commit['num']} ({commit['sha']})")
    print(f"   Average impact: {commit['avg_pct']:+.2f}% ({impact_type})")
    print(f"   Worst regression: {commit['max_regression']:+.2f}%")
    print(f"   Best improvement: {commit['max_improvement']:+.2f}%")
    print()

print()
print("=" * 80)
print("BIGGEST REGRESSIONS (Performance got worse)")
print("=" * 80)
print()

regressions = [c for c in commit_impacts if c['avg_pct'] > 1.0]
regressions.sort(key=lambda x: x['avg_pct'], reverse=True)

for i, commit in enumerate(regressions[:10], 1):
    print(f"{i}. Commit {commit['num']} ({commit['sha']}): {commit['avg_pct']:+.2f}% average")

print()
print("=" * 80)
print("BIGGEST IMPROVEMENTS (Performance got better)")
print("=" * 80)
print()

improvements = [c for c in commit_impacts if c['avg_pct'] < -1.0]
improvements.sort(key=lambda x: x['avg_pct'])

for i, commit in enumerate(improvements[:10], 1):
    print(f"{i}. Commit {commit['num']} ({commit['sha']}): {commit['avg_pct']:.2f}% average")

