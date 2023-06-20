local filename = "bin/SparkAmpOS"

-- Open the file in binary mode
local file = io.open(filename, "rb")

-- ANSI escape sequences
local blue = "\27[34m"
local red = "\27[31m"
local reset = "\27[0m"

if file then
	file:seek("end")
	local size = file:seek()

	file:close()
	io.write(blue .. "File Size: " .. size .. " bytes" .. reset .. "\n")
	io.write(blue .. "Sectors to read: " .. math.ceil(size / 512) .. reset .. "\n")
else
	io.write(red .. "Failed to open the file" .. reset .. "\n")
end
