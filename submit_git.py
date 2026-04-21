import os
import json
from acmoj_client import ACMOJClient

token = os.environ.get("ACMOJ_TOKEN")
problem_id = 2100
git_url = "https://github.com/ojbench/oj-eval-gemini-cli-033-20260421112222"

client = ACMOJClient(token)
result = client.submit_git(problem_id, git_url)
print(json.dumps(result))
