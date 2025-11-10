#!/usr/bin/env bash
# codex-fetch-now.sh
# One-time fetch of Codex reviews without polling
#
# Usage: ./codex-fetch-now.sh [PR_NUMBER]
#
# This script does an immediate one-time check for Codex reviews and displays them.
# Unlike codex-auto-monitor.sh check, this fetches fresh data from GitHub instead
# of reading from the polling logs.

set -euo pipefail

PR_NUMBER="${1:-}"
REPO="${GITHUB_REPOSITORY:-JesseBuesking/omnomnum}"

# Auto-detect PR number from current branch if not provided
auto_detect_pr() {
  local branch=$(git branch --show-current 2>/dev/null)
  if [ -z "$branch" ]; then
    echo ""
    return 1
  fi

  if command -v gh &> /dev/null; then
    local pr=$(gh pr list --head "$branch" --json number --jq '.[0].number' 2>/dev/null)
    if [ -n "$pr" ]; then
      echo "$pr"
      return 0
    fi
  fi

  echo ""
  return 1
}

usage() {
  echo "Usage: $0 [PR_NUMBER]"
  echo ""
  echo "One-time fetch of Codex reviews without polling."
  echo "Auto-detects PR from current branch if not provided."
  echo ""
  echo "Examples:"
  echo "  $0           # Auto-detect PR from current branch"
  echo "  $0 36        # Check PR #36"
  exit 1
}

# Fetch GitHub API with curl or gh CLI
fetch_github_api() {
  local endpoint="$1"

  if [ -n "${GITHUB_TOKEN:-}" ]; then
    # Add github_pat_ prefix if token doesn't already have a prefix
    local token="$GITHUB_TOKEN"
    if [[ ! "$token" =~ ^(ghp_|github_pat_) ]]; then
      token="github_pat_${token}"
    fi

    # Use curl with token
    curl -s -H "Authorization: token $token" \
         -H "Accept: application/vnd.github+json" \
         "https://api.github.com/${endpoint}"
  elif command -v gh &> /dev/null; then
    # Fall back to gh CLI
    gh api "$endpoint" 2>/dev/null || echo "[]"
  else
    echo "Error: No authentication method available (GITHUB_TOKEN or gh CLI)" >&2
    exit 1
  fi
}

# Main execution
if [ "$PR_NUMBER" = "-h" ] || [ "$PR_NUMBER" = "--help" ]; then
  usage
fi

# Auto-detect PR if not provided
if [ -z "$PR_NUMBER" ]; then
  PR_NUMBER=$(auto_detect_pr || true)
  if [ -z "$PR_NUMBER" ]; then
    echo "Error: Could not auto-detect PR number from current branch."
    echo "Please provide PR_NUMBER manually."
    exit 1
  fi
  echo "Auto-detected PR #${PR_NUMBER} from current branch"
  echo ""
fi

echo "Fetching Codex reviews for PR #${PR_NUMBER}..."
echo ""

# Fetch all comment types that might contain Codex reviews
echo "Checking for Codex reviews..."

# 1. Issue comments (on PR)
ISSUE_COMMENTS=$(fetch_github_api "repos/${REPO}/issues/${PR_NUMBER}/comments")

# 2. Review comments (on specific code lines)
REVIEW_COMMENTS=$(fetch_github_api "repos/${REPO}/pulls/${PR_NUMBER}/comments")

# 3. PR reviews (full reviews)
PR_REVIEWS=$(fetch_github_api "repos/${REPO}/pulls/${PR_NUMBER}/reviews")

# Process and display Codex reviews
FOUND_P0=0
FOUND_P1=0
FOUND_ANY=0

# Look for Codex badge pattern in all comments
process_comment() {
  local body="$1"
  local type="$2"
  local id="$3"

  # Check if comment contains Codex badges
  if echo "$body" | grep -q "Badge"; then
    FOUND_ANY=1

    # Check for P0/P1/P2 badges
    if echo "$body" | grep -q "P0 Badge"; then
      FOUND_P0=1
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo "🚨 P0 CRITICAL ISSUE FOUND ($type #$id)"
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo ""
      echo "$body"
      echo ""
    elif echo "$body" | grep -q "P1 Badge"; then
      FOUND_P1=1
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo "🔴 P1 ISSUE FOUND ($type #$id)"
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo ""
      echo "$body"
      echo ""
    elif echo "$body" | grep -q "P2 Badge"; then
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo "🟡 P2 Issue ($type #$id)"
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo ""
      echo "$body"
      echo ""
    else
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo "✅ Codex Review ($type #$id)"
      echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
      echo ""
      echo "$body"
      echo ""
    fi
  fi
}

# Process issue comments (use process substitution to avoid subshell)
while IFS='||||' read -r id body; do
  if [ -n "$id" ] && [ -n "$body" ]; then
    process_comment "$body" "Issue Comment" "$id"
  fi
done < <(echo "$ISSUE_COMMENTS" | jq -r '.[] | "\(.id)||||\(.body)"' 2>/dev/null)

# Process review comments (use process substitution to avoid subshell)
while IFS='||||' read -r id body; do
  if [ -n "$id" ] && [ -n "$body" ]; then
    process_comment "$body" "Review Comment" "$id"
  fi
done < <(echo "$REVIEW_COMMENTS" | jq -r '.[] | "\(.id)||||\(.body)"' 2>/dev/null)

# Process PR reviews (use process substitution to avoid subshell)
while IFS='||||' read -r id body; do
  if [ -n "$id" ] && [ -n "$body" ]; then
    process_comment "$body" "PR Review" "$id"
  fi
done < <(echo "$PR_REVIEWS" | jq -r '.[] | "\(.id)||||\(.body // "")"' 2>/dev/null)

# Summary
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if [ $FOUND_P0 -eq 1 ]; then
  echo "🚨 P0 CRITICAL issues found! Please address immediately."
  exit 1
elif [ $FOUND_P1 -eq 1 ]; then
  echo "⚠️  P1 issues found! Please address them."
  exit 1
elif [ $FOUND_ANY -eq 1 ]; then
  echo "✅ Codex reviews found (no P0/P1 issues)"
  exit 0
else
  echo "ℹ️  No Codex reviews found yet"
  exit 0
fi
