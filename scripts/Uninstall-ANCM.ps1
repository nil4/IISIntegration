param($ModuleImage)

. "$PSScriptRoot\common.ps1"

Start-IISCommitDelay

$OldModule = GetOldANCMModule;
if (!($OldModule))
{
    Write-Host -ForegroundColor Red "OldAspNetCoreModule module not found";
    Break;
}

$Module = GetANCMModule;
if ($Module)
{
    Write-Host "Removing AspNetCoreModule";
    Remove-IISConfigElement -ConfigElement $Module;
}

Stop-IISCommitDelay -Commit $True


Start-IISCommitDelay

Write-Host "Renaming OldAspNetCoreModule to AspNetCoreModule";
$OldModule = GetOldANCMModule;
Set-IISConfigAttributeValue -ConfigElement $OldModule -AttributeName "name" -AttributeValue "AspNetCoreModule"

Stop-IISCommitDelay -Commit $True
