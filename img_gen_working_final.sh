#!/bin/bash

# Color definitions
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color
BOLD='\033[1m'

# Configuration
CLOUDFLARE_ACCOUNT_ID="d03ba4adab44b19322eed9dd5a40de7d"
CLOUDFLARE_AUTH_TOKEN="MkPwb3osbvgKuSWn8XR43dnk3vAs2VW-EgXZ_pek"
FB_PAGE_ID="555233441009283"
FB_ACCESS_TOKEN="EAAdBZBbj3fdwBO69Ltwr34trvRwRCHhe5vkgyfMYIMnrbfELEuVAK7EEdccGYDQ4Lq0a6dwZCjGigVyaIiDZBdpsU1wLEBXMu9vi7pA7iT6ZBiZCNQJZAj5qDMf8CD8tLXHhlR1mFd8m6yZCREskVsOZB2dOMf0dKzZBNGXYLJVfSgdDmt70KaNPZCqCIW3WU1YiiU"

# Function to print banner
print_banner() {
  echo -e "${CYAN}${BOLD}"
  echo "╔════════════════════════════════════════╗"
  echo "║     Family Island Image Generator      ║"
  echo "╚════════════════════════════════════════╝"
  echo -e "${NC}"
}

# Function to log messages with timestamps
log_message() {
  local level=$1
  local message=$2
  local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
  case $level in
  "INFO")
    echo -e "${BLUE}[INFO]${NC} ${timestamp} - $message"
    ;;
  "SUCCESS")
    echo -e "${GREEN}[SUCCESS]${NC} ${timestamp} - $message"
    ;;
  "ERROR")
    echo -e "${RED}[ERROR]${NC} ${timestamp} - $message"
    ;;
  "WARN")
    echo -e "${YELLOW}[WARN]${NC} ${timestamp} - $message"
    ;;
  esac
}

# Function to check required commands
check_dependencies() {
  local deps=("curl" "jq" "base64")
  local missing=()

  for dep in "${deps[@]}"; do
    if ! command -v "$dep" &>/dev/null; then
      missing+=("$dep")
    fi
  done

  if [ ${#missing[@]} -ne 0 ]; then
    log_message "ERROR" "Missing required dependencies: ${missing[*]}"
    echo -e "${RED}Please install the missing dependencies and try again.${NC}"
    exit 1
  fi
}

# Function to validate environment variables
validate_config() {
  local required_vars=("CLOUDFLARE_ACCOUNT_ID" "CLOUDFLARE_AUTH_TOKEN" "FB_PAGE_ID" "FB_ACCESS_TOKEN")
  local missing=()

  for var in "${required_vars[@]}"; do
    if [ -z "${!var}" ]; then
      missing+=("$var")
    fi
  done

  if [ ${#missing[@]} -ne 0 ]; then
    log_message "ERROR" "Missing required environment variables: ${missing[*]}"
    exit 1
  fi
}

# Main execution with error handling
main() {
  print_banner
  check_dependencies
  validate_config

  log_message "INFO" "Starting image generation process..."

  # Prepare JSON payload
  JSON_PAYLOAD=$(
    cat <<EOF
{
  "messages": [
    { "role": "system", "content": "You are a friendly assistant" },
    { "role": "user", "content": "suggest a single prompt dont output anything other than the prompt, the prompt will be given to a text to image generation ai, the image is about Family island game and free energy which is a item inside the game, here is some context about the game: Immerse yourself in an unforgettable world of adventure with a modern Stone Age family! Imagine what your life would be without modern technology, what would you do? Perhaps you would explore territories, build houses or even entire villages; farm, harvest, or maybe you would conquer new lands. You have an opportunity to be on a desert island together with the heroes of the Family Island game and try yourself in different roles: farmer, cook, explorer, trader and many others. Interested? Here are some more features of our game: ★ Explore wild territories, solve puzzles, find hidden objects and set on a thrilling adventure to new islands. ★ Build and improve your little city in the middle of the ocean. ★ Start your own family farm! Harvest, grow crops and craft useful goods to trade with other characters. ★ Cook healthy and tasty food from the ingredients that you can find on the island. ★ Customize your village with beautiful decorations! Choose flowers and plants that match the unusual landscapes of your village. ★ Meet unusual animals: island hamsters, wild goats and even a dinosaur are waiting for you! ★ Help a family survive on a desert island. And that is not all! Family Island is a farm game full of unexpected twists and captivating adventure!" }
  ]
}
EOF
  )

  # Get prompt from Cloudflare AI
  log_message "INFO" "Generating prompt using Cloudflare AI..."
  prompt=$(curl -s https://api.cloudflare.com/client/v4/accounts/$CLOUDFLARE_ACCOUNT_ID/ai/run/@cf/meta/llama-3.1-70b-instruct \
    -X POST \
    -H "Authorization: Bearer $CLOUDFLARE_AUTH_TOKEN" \
    -H "Content-Type: application/json" \
    -d "$JSON_PAYLOAD" | jq -r '.result.response')

  if [ -z "$prompt" ]; then
    log_message "ERROR" "Failed to generate prompt"
    exit 1
  fi

  log_message "SUCCESS" "Prompt generated successfully"

  # Generate image
  log_message "INFO" "Generating image using prompt..."
  current_date=$(date +"%d %B %Y")
  response=$(curl -s https://api.cloudflare.com/client/v4/accounts/$CLOUDFLARE_ACCOUNT_ID/ai/run/@cf/black-forest-labs/flux-1-schnell \
    -X POST \
    -H "Authorization: Bearer $CLOUDFLARE_AUTH_TOKEN" \
    -H "Content-Type: application/json" \
    -d "$(jq -nc --arg p "$prompt" '{prompt: $p}')")

  # Extract and save image
  base64_image=$(echo "$response" | jq -r '.result.image')
  if [ -z "$base64_image" ]; then
    log_message "ERROR" "Failed to generate image"
    exit 1
  fi

  echo "$base64_image" | base64 --decode >result.jpg
  if [ ! -f "result.jpg" ]; then
    log_message "ERROR" "Failed to save image"
    exit 1
  fi

  log_message "SUCCESS" "Image generated and saved successfully"

  # Upload to Facebook
  log_message "INFO" "Uploading to Facebook..."
  facebook_response=$(curl -s -X POST "https://graph.facebook.com/v22.0/$FB_PAGE_ID/photos" \
    -F "access_token=$FB_ACCESS_TOKEN" \
    -F "source=@result.jpg" \
    -F "caption=Family Island Free Energy ⚡🎁 
($current_date) 
https://gogl.to/3GEF ✅")

  if echo "$facebook_response" | grep -q "id"; then
    log_message "SUCCESS" "Image uploaded to Facebook successfully!"
    post_id=$(echo "$facebook_response" | jq -r '.id')
    log_message "INFO" "Post ID: $post_id"
  else
    log_message "ERROR" "Failed to upload image to Facebook"
    log_message "ERROR" "Response: $facebook_response"
    exit 1
  fi
}

# Trap errors
trap 'echo -e "\n${RED}Script execution failed${NC}"; exit 1' ERR

# Run main function
main
