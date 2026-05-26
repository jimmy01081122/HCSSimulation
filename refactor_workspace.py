import os
import re
import shutil

# Emojis regex pattern
EMOJI_PATTERN = re.compile(
    u'[\U00010000-\U0010ffff]|'
    u'[\u2600-\u27FF]|'
    u'[\u2B50-\u2B55]|'
    u'[\u231A-\u23FA]'
)

def strip_emojis(text):
    return EMOJI_PATTERN.sub(r'', text)

    lines = text.split('\n')
    filtered = []
    for line in lines:
        if 'x86' in line.lower() and 'riscv' not in line.lower() and 'arm' not in line.lower():
        filtered.append(line)
    return '\n'.join(filtered)

def process_file(filepath):
    try:
        with open(filepath, 'r', encoding='utf-8') as f:
            content = f.read()
    except Exception as e:
        print(f"Skipping {filepath} due to read error: {e}")
        return

    # 1. Strip emojis
    content = strip_emojis(content)
    # 3. Update gem5 clone URL if it mentions an old one
    content = content.replace("git clone https://github.com/gem5/gem5", "git clone https://github.com/gem5/gem5")
    # Some older docs might mention "git clone" with a tag
    # For now, just ensure it uses the github URL

    with open(filepath, 'w', encoding='utf-8') as f:
        f.write(content)

def walk_and_process(directory):
    for root, dirs, files in os.walk(directory):
        if 'gem5' in root.split(os.sep) or '.git' in root.split(os.sep):
            continue
        for file in files:
            if file.endswith(('.md', '.py', '.sh', '.txt')):
                process_file(os.path.join(root, file))

def create_topic_structure():
    topics = [
        "01_se_mode",
        "02_fs_mode",
        "03_standard_library",
        "04_python_config",
        "05_cpu_model",
        "06_cache_hierarchy",
        "07_memory_system",
        "08_ruby_coherence",
        "09_statistics_parsing",
        "10_design_space_exploration"
    ]
    base_dir = "topics"
    os.makedirs(base_dir, exist_ok=True)
    
    for topic in topics:
        topic_dir = os.path.join(base_dir, topic)
        os.makedirs(topic_dir, exist_ok=True)
        
        # Create run script
        run_script_path = os.path.join(topic_dir, "run.sh")
        with open(run_script_path, 'w', encoding='utf-8') as f:
            f.write("#!/bin/bash\n")
            f.write(f"# 執行 {topic} 測試\n")
            f.write("echo \"Running gem5 simulation for " + topic + "...\"\n")
            f.write("# TODO: 替換為實際的gem5執行指令\n")
            f.write("# /work/gem5/build/ARM/gem5.opt config.py\n")
        os.chmod(run_script_path, 0o755)

        # Create python config skeleton
        config_path = os.path.join(topic_dir, f"{topic[3:]}_test.py")
        with open(config_path, 'w', encoding='utf-8') as f:
            f.write('"""\n')
            f.write(f"簡述: 此為 {topic} 的測試配置檔案，用於驗證相關功能。\n")
            f.write('"""\n\n')
            f.write("import m5\n")
            f.write("from m5.objects import *\n\n")
            f.write("# TODO: 實作系統配置 (System configuration)\n")
            f.write("# system = System()\n\n")
            f.write("# TODO: 實作 CPU 與 Memory 配置\n\n")
            f.write("# TODO: 實作 Root 並開始模擬 (m5.instantiate())\n")
            f.write("print('Simulation logic goes here.')\n")
            
        # Create README
        readme_path = os.path.join(topic_dir, "README.md")
        with open(readme_path, 'w', encoding='utf-8') as f:
            f.write(f"# {topic}\n\n")
            f.write("這是一份關於此主題的教學文件。\n\n")
            f.write("## 閱讀順序與實作\n")
            f.write("1. 閱讀此 README\n")
            f.write(f"2. 完成 `{topic[3:]}_test.py` 中的 TODO\n")
            f.write("3. 執行 `./run.sh` 進行測試\n")

if __name__ == "__main__":
    print("Deleting old directories...")
    if os.path.exists('experiments'):
        shutil.rmtree('experiments')
    if os.path.exists('dir'):
        shutil.rmtree('dir')
        
    walk_and_process('.')
    
    print("Creating topic structure...")
    create_topic_structure()
    print("Done.")
